package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"hash/fnv"
	"io"
	"os"
)

// KeyValuePair представляет пару ключ-значение
type KeyValuePair struct {
	Key   string
	Value string
	Next  *KeyValuePair
}

// DynamicArray представляет динамический массив для хранения указателей на KeyValuePair
type DynamicArray struct {
	array    []*KeyValuePair
	size     int
	capacity int
}

// NewDynamicArray создает новый DynamicArray с начальной емкостью
func NewDynamicArray(initialCapacity int) *DynamicArray {
	return &DynamicArray{
		array:    make([]*KeyValuePair, initialCapacity),
		size:     0,
		capacity: initialCapacity,
	}
}

// Resize изменяет размер массива
func (da *DynamicArray) Resize(newCapacity int) {
	newArray := make([]*KeyValuePair, newCapacity)
	copy(newArray, da.array)
	da.array = newArray
	da.capacity = newCapacity
}

// Add добавляет элемент в массив
func (da *DynamicArray) Add(element *KeyValuePair) {
	if da.size == da.capacity {
		da.Resize(da.capacity * 2)
	}
	da.array[da.size] = element
	da.size++
}

// Get возвращает элемент по индексу
func (da *DynamicArray) Get(index int) *KeyValuePair {
	if index < 0 || index >= da.capacity {
		return nil
	}
	return da.array[index]
}

// Set устанавливает элемент по индексу
func (da *DynamicArray) Set(index int, element *KeyValuePair) {
	if index < 0 || index >= da.capacity {
		return
	}
	da.array[index] = element
}

// Size возвращает текущий размер массива
func (da *DynamicArray) Size() int {
	return da.size
}

// Capacity возвращает текущую емкость массива
func (da *DynamicArray) Capacity() int {
	return da.capacity
}

// HashTable представляет хеш-таблицу
type HashTable struct {
	table *DynamicArray
}

// NewHashTable создает новую хеш-таблицу с начальной емкостью
func NewHashTable() *HashTable {
	return &HashTable{
		table: NewDynamicArray(10),
	}
}

// hashFunction вычисляет хеш для строки
func (ht *HashTable) hashFunction(key string) int {
	h := fnv.New32a()
	h.Write([]byte(key))
	return int(h.Sum32()) % ht.table.Capacity()
}

// Insert вставляет пару ключ-значение в хеш-таблицу
func (ht *HashTable) Insert(key string, value string) {
	index := ht.hashFunction(key)
	current := ht.table.Get(index)

	// Проверяем, существует ли уже элемент с таким ключом
	for current != nil {
		if current.Key == key {
			// Если ключ уже существует, обновляем значение
			current.Value = value
			return
		}
		current = current.Next
	}

	// Если ключ не найден, добавляем новый элемент
	newPair := &KeyValuePair{Key: key, Value: value}
	if ht.table.Get(index) == nil {
		// Если ячейка пуста, просто добавляем новую пару
		ht.table.Set(index, newPair)
	} else {
		// Если ячейка занята, добавляем новую пару в начало списка
		newPair.Next = ht.table.Get(index)
		ht.table.Set(index, newPair)
	}
}

// Get получает значение по ключу
func (ht *HashTable) Get(key string) string {
	index := ht.hashFunction(key)
	current := ht.table.Get(index)

	for current != nil {
		if current.Key == key {
			return current.Value
		}
		current = current.Next
	}

	return "" // Возвращаем пустую строку, если ключ не найден
}

// Remove удаляет пару ключ-значение по ключу
func (ht *HashTable) Remove(key string) {
	index := ht.hashFunction(key)
	current := ht.table.Get(index)
	var prev *KeyValuePair

	for current != nil && current.Key != key {
		prev = current
		current = current.Next
	}

	if current == nil {
		// Ключ не найден
		return
	}

	if prev == nil {
		// Если удаляем первый элемент в списке
		ht.table.Set(index, current.Next)
	} else {
		// Если удаляем не первый элемент в списке
		prev.Next = current.Next
	}
}

// Serialize сериализует хеш-таблицу в бинарный файл
func (ht *HashTable) Serialize(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	buf := new(bytes.Buffer)

	// Записываем размер таблицы
	capacity := ht.table.Capacity()
	if err := binary.Write(buf, binary.LittleEndian, int32(capacity)); err != nil {
		return err
	}

	// Записываем каждую пару ключ-значение
	for i := 0; i < capacity; i++ {
		current := ht.table.Get(i)
		for current != nil {
			// Записываем ключ
			keyBytes := []byte(current.Key)
			if err := binary.Write(buf, binary.LittleEndian, int32(len(keyBytes))); err != nil {
				return err
			}
			if _, err := buf.Write(keyBytes); err != nil {
				return err
			}

			// Записываем значение
			valueBytes := []byte(current.Value)
			if err := binary.Write(buf, binary.LittleEndian, int32(len(valueBytes))); err != nil {
				return err
			}
			if _, err := buf.Write(valueBytes); err != nil {
				return err
			}

			current = current.Next
		}
	}

	// Записываем буфер в файл
	if _, err := file.Write(buf.Bytes()); err != nil {
		return err
	}

	return nil
}

// Deserialize десериализует хеш-таблицу из бинарного файла
func (ht *HashTable) Deserialize(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var capacity int32
	if err := binary.Read(file, binary.LittleEndian, &capacity); err != nil {
		return err
	}

	ht.table = NewDynamicArray(int(capacity))

	for i := 0; i < int(capacity); i++ {
		for {
			var keyLen int32
			if err := binary.Read(file, binary.LittleEndian, &keyLen); err != nil {
				if err == io.EOF {
					return nil
				}
				return err
			}

			keyBytes := make([]byte, keyLen)
			if _, err := file.Read(keyBytes); err != nil {
				return err
			}

			key := string(keyBytes)

			var valueLen int32
			if err := binary.Read(file, binary.LittleEndian, &valueLen); err != nil {
				return err
			}

			valueBytes := make([]byte, valueLen)
			if _, err := file.Read(valueBytes); err != nil {
				return err
			}

			value := string(valueBytes)

			ht.Insert(key, value)
		}
	}

	return nil
}

func main() {
	// Пример использования с хеш-таблицей
	hashTable := NewHashTable()
	hashTable.Insert("key1", "value1")
	hashTable.Insert("key2", "value2")
	hashTable.Insert("key3", "value3")

	// Сериализация хеш-таблицы
	if err := hashTable.Serialize("hashtable.bin"); err != nil {
		fmt.Println("Error serializing hash table:", err)
		return
	}

	// Десериализация хеш-таблицы
	hashTable2 := NewHashTable()
	if err := hashTable2.Deserialize("hashtable.bin"); err != nil {
		fmt.Println("Error deserializing hash table:", err)
		return
	}

	// Вывод элементов хеш-таблицы
	fmt.Println("key1:", hashTable2.Get("key1"))
	fmt.Println("key2:", hashTable2.Get("key2"))
	fmt.Println("key3:", hashTable2.Get("key3"))
}
