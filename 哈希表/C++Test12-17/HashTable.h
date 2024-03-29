#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace sy
{
	enum State
	{
		EMPTY,
		EXIST,
		DELETE
	};

	class dealInt
	{
	public:
		int operator()(int n)
		{
			return n;
		}
	};

	class dealString
	{
	public:
		int operator()(const string & s)
		{
			int seed = 131;
			int sum = 0;

			for (auto & e : s)
			{
				sum = sum * seed + e;
			}
			return sum & 0x7FFFFFFF;
		}
	};

	template <class K, class V, class SW = dealInt>
	class hashTable
	{
		struct elem
		{
			pair<K, V> m_val;
			State m_state;

			elem(const K & key = K(), const V & val = V(), State state = EMPTY) :
				m_val(key, val),
				m_state(state)
			{

			}
		};

		vector<elem> m_table;
		size_t m_size;
	public:

		hashTable(size_t capacity = 11) :
			m_table(capacity),
			m_size(0)
		{

		}

		size_t capacity()
		{
			return m_table.size();
		}
	private:
		//哈希函数
		int hashFunc(const K & key)
		{
			SW func;

			return func(key) % capacity();
		}

	public:
		//插入函数
		bool insert(const pair<K, V> & val)
		{
			int n = hashFunc(val.first);

			while (m_table[n].m_state == EXIST)
			{

				if (m_table[n].m_val.first == val.first)
				{
					return false;
				}

				++n;
				if (n == capacity())
				{
					n = 0;
				}
			}

			m_table[n].m_val = val;
			m_table[n].m_state = EXIST;
			++m_size;
			return true;
		}

		//find是为了方便删除, 找到后返回下标
		int find(const K & key)
		{
			int n = hashFunc(key);

			while (m_table[n].m_state != EMPTY)
			{
				if (m_table[n].m_state == EXIST && m_table[n].m_val.first == key)
				{
					return n;
				}
				++n;

				if (n == capacity())
				{
					n = 0;
				}
			}

			return -1;
		}

		bool erase(const K & key)
		{
			int ret = find(key);

			if (ret < 0)
			{
				return false;
			}
			else
			{
				m_table[ret].m_state = DELETE;
				m_size--;
			}
			return true;
		}

		size_t size()
		{
			return m_size;
		}

		bool empty()
		{
			return m_size == 0;
		}

		void Swap(hashTable<K, V> & ht)
		{
			m_table.swap(ht.m_table);

			size_t tmp;
			tmp = m_size;
			m_size = ht.m_size;
			ht.m_size = tmp;
		}

	};
};