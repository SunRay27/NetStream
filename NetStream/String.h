#pragma once

class String
{
private:
	size_t length;
	char* value;

	void RewriteArray(const String& from)
	{
		if (this->value != nullptr)
			delete[] this->value;

		char* copy = new char[from.length + 1];
		for (size_t i = 0; i < from.length; i++)
			copy[i] = from.value[i];

		copy[from.length] = '\0';
		this->length = from.length;
		this->value = copy;
	}

	char* GetArrayCopy()
	{
		char* val = new char[length + 1];
		for (size_t i = 0; i < length; i++)
			val[i] = value[i];
		val[length] = '\0';

		return val;
	}
public:
	size_t Length()
	{
		return length;
	}

	
	char* GetArrayPointer() const
	{
		return value;
	}

	//CONSTRUCTORS
	//copy constructor
	String(const String& other)
	{
		char* copy = new char[other.length + 1];
		for (size_t i = 0; i < other.length; i++)
			copy[i] = other.value[i];
		copy[other.length] = '\0';

		length = other.length;
		value = copy;
	}
	String(const char* value)
	{
		length = 0;
		while (value[length] != '\0')
			length++;

		char* copy = new char[length+1];
		for (size_t i = 0; i < length; i++)
			copy[i] = value[i];
		copy[length] = '\0';
		this->value = copy;
	}
	String(const char value)
	{
		length = 1;
		char* copy = new char[2];
		copy[0] = value;
		copy[1] = '\0';

		this->value = copy;
	}
	String()
	{
		length = 0;
		value = new char[1];
		value[0] = '\0';
	}
	~String()
	{
		if (value != nullptr)
			delete[] value;
	}

	//Operator overloads
	friend ostream& operator<<(ostream& stream, const String& string)
	{
		//stream << "\"";
		for (size_t i = 0; i < string.length; i++)
		{
			//if (string.value[i] < 32 || string.value[i] > 127)
			//	stream << '?';
			//else
				stream << string.value[i];
		}
		//stream << "\"";
		return stream;
	}

	bool operator<(const String& other)
	{
		int compResult = strcmp(value, other.value);
		return compResult < 0;
	}
	bool operator>(const String& other)
	{
		int compResult = strcmp(value, other.value);
		return compResult > 0;
	}
	bool operator==(const String& toCompare)
	{
		return Equals(toCompare);
	}
	bool operator!=(const String& toCompare)
	{
		return !(* this == toCompare);
	}

	String& operator=(const String& other)
	{
		if (this != &other)
		{
			length = other.length;
			char* copy = new char[length + 1];
			for (size_t i = 0; i < length; i++)
				copy[i] = other.value[i];
			copy[length] = '\0';

			delete[] value;
			this->value = copy;
		}
		return *this;
	}
	String& operator+(const char* other)
	{
		String returnVal = *this;
		returnVal.Concat(other);
		return returnVal;
	}
	String& operator+(const String& other)
	{
		String returnVal = *this;
		returnVal.Concat(other);
		return returnVal;
	}
	String& operator+=(const String& other)
	{
		Concat(other);
		return *this;
	}
	String& operator+=(const char* other)
	{
		Concat(other);
		return *this;
	}
	String& operator+=(char other)
	{
		Concat(other);
		return *this;
	}

	char& operator[] (size_t index) const
	{
		return value[index];
	}
	





	//concatenation functions
	void Concat(const char* toAdd)
	{
		size_t addLen = 0;
		while (toAdd[addLen] != '\0')
			addLen++;

		char* newVal = new char[length + addLen + 1];

		for (size_t i = 0; i < length; i++)
			newVal[i] = value[i];
		for (size_t i = 0; i < addLen; i++)
			newVal[i + length] = toAdd[i];

		newVal[length + addLen] = '\0';

		delete[] value;
		value = newVal;
		length = length + addLen;
	}
	void Concat(char toAdd)
	{
		char* newVal = new char[length + 2];

		for (size_t i = 0; i < length; i++)
			newVal[i] = value[i];
		newVal[length] = toAdd;
		newVal[length + 1] = '\0';

		length++;
		if (value != nullptr)
			delete[] value;
		value = newVal;
	}
	void Concat(const String& toAdd)
	{
		size_t addLen = toAdd.length;
		char* newVal = new char[length + addLen + 1];

		for (size_t i = 0; i < length; i++)
			newVal[i] = value[i];
		for (size_t i = 0; i < addLen; i++)
			newVal[i + length] = toAdd.value[i];

		newVal[length + addLen] = '\0';

		delete[] value;
		value = newVal;
		length = length + addLen;
	}

	//... another useless comment
	bool Equals(const char* toCompare)
	{
		if (strlen(toCompare) != length)
			return false;

		for (size_t i = 0; i < length; i++)
			if (value[i] != toCompare[i])
				return false;

		return true;
	}
	bool Equals(const String& toCompare)
	{
		if (toCompare.length != length)
			return false;
		for (size_t i = 0; i < length; i++)
			if (value[i] != toCompare[i])
				return false;

		return true;
	}
	static bool Equals(const String& l, const String& r)
	{
		if (r.length != l.length)
			return false;
		for (size_t i = 0; i < l.length; i++)
			if (l[i] != r[i])
				return false;

		return true;
	}

	bool Contains(char toCompare)
	{
		for (size_t i = 0; i < length; i++)
			if (value[i] == toCompare)
				return true;

		return false;
	}
	bool Contains(const String& toCompare)
	{
		if (toCompare.length > length)
			return false;

		for (size_t i = 0; i < length; i++)
		{
			if (value[i] == toCompare[0])
			{
				bool areDifferent = false;
				//first symbol is equal, check the rest
				for (size_t j = 1; j < toCompare.length; j++)
				{
					if (value[i + j] != toCompare[j])
					{
						areDifferent = true;
						break;
					}
				}
				if (!areDifferent)
					return true;
			}
		}
		return false;
	}


	void Trim()
	{
		if (length == 0)
			return;

		
		size_t startIndex = 0;
		size_t endIndex = length-1;
		while (startIndex < length - 1 && value[startIndex] == ' ' )
			startIndex++;

		while (endIndex > 0 && value[endIndex] == ' ')
			endIndex--;
		size_t newSize = endIndex - startIndex + 1;

		char* copy = new char[newSize + 1];
		
		size_t count = 0;
		for (size_t i = startIndex; i < endIndex + 1; i++)
		{
			copy[count] = value[i];
			count++;
		}
		copy[newSize] = '\0';

		delete[] value;
		value = copy;
		length = newSize;
	}



	//Lazy concat implementation
	String Substring(size_t startIndex, size_t endIndex)
	{
		String result;
		for (size_t i = startIndex; i < endIndex; i++)
			result += value[i];
		return result;
	}
	String Substring(size_t startIndex)
	{
		String result;
		for (size_t i = startIndex; i < length; i++)
			result += value[i];
		return result;
	}


	void RemoveAt(size_t index)
	{
		length--;

		char* copy = new char[length+1];
		for (size_t i = 1; i < length+1; i++)
			copy[i] = value[i];
		copy[length] = '\0';

		value = copy;
	}

	//replaces first entry of oldValue to newValue
	void ReplaceFirst(String oldValue, String newValue)
	{
		if (Contains(oldValue))
		{
			//find start index
			size_t startIndex = -1;
			for (size_t i = 0; i < length; i++)
			{
				if (value[i] == oldValue[0])
				{
					bool areDifferent = false;
					//first symbol is equal, check the rest
					for (size_t j = 1; j < oldValue.length; j++)
					{
						if (oldValue[j] != value[i + j])
						{
							areDifferent = true;
							break;
						}
					}
					if (!areDifferent)
					{
						startIndex = i;
						break;
					}
				}
			}

			//split string into 2 pieces between oldValue
			String first = Substring(0, startIndex);
			String second = Substring(startIndex + oldValue.length);

			first += newValue + second;

			RewriteArray(first);
		}
	}
	//replaces all occurences of oldValue with newValue
	void ReplaceAll(String oldValue, String newValue)
	{
		while (Contains(oldValue))
			ReplaceFirst(oldValue, newValue);
	}



	//static functions
	static List<String> LazySplit (const String& source, char separator)
	{
		List<String> result;
		size_t newStringCount = 1, length = source.length;
		char* value = source.value;

		for (size_t i = 0; i < length; i++)
			if (value[i] == separator)
				newStringCount++;

		size_t* markers = new size_t[newStringCount];
		markers[newStringCount - 1] = length;

		//setup markers
		size_t counter = 0;
		for (size_t i = 0; i < length; i++)
		{
			if (value[i] == separator)
			{
				markers[counter] = i;
				counter++;
			}
		}

		size_t markerIndex = -1;
		size_t lastMarkerIndex = -1;
		for (size_t i = 0; i < newStringCount; i++)
		{
			markerIndex++;
			String segment;
			for (size_t j = lastMarkerIndex + 1; j < length; j++)
			{
				if (j > length - 1)
					break;

				if (j < markers[markerIndex])
					segment += (value[j]);
				else
					break;
			}
			if (segment.length > 0)
				result.Add(segment);

			lastMarkerIndex = markers[markerIndex];
		}

		delete[] markers;
		return result;
	}

	static String FromDouble(double value)
	{
		char c[50];
		sprintf_s(c, "%f", value);
		//String toReturn(c);
		return String(c);
	}

	//bit add-on for some lab
	class ShannonFanoEncodedStringBitIterator
	{
	private:
		const int countByteSize = 8;
		size_t currentBitPos;
		String* str;
	public:
		size_t GetOriginalStringLength()
		{
			//remember... first 8 bytes...
			//it's just better for everyone's eyes to access pretty-named var than accessing str->value[i]
			char* buffer = new char[countByteSize];
			for (size_t i = 0; i < countByteSize; i++)
				buffer[i] = str->value[i];

			//yeah, that is how we do programs in 2022
			//and I'm happy about it
			//probably...
		 size_t size = (((size_t)buffer[0] << 56) & 0xff00000000000000u) |
					   (((size_t)buffer[1] << 48) & 0x00ff000000000000u) |
					   (((size_t)buffer[2] << 40) & 0x0000ff0000000000u) |
					   (((size_t)buffer[3] << 32) & 0x000000ff00000000u) |
					   ((buffer[4] << 24)		  & 0x00000000ff000000u) |
					   ((buffer[5] << 16)		  & 0x0000000000ff0000u) |
					   ((buffer[6] << 8)		  & 0x000000000000ff00u) |
					   ( buffer[7]				  & 0x00000000000000ffu);
				
			//cout << "\ngot " << size << " from 8 bytes...\n";

			delete buffer;
			return size;
		}
		bool Next()
		{
			return (str->value[(currentBitPos / 8)] >> (currentBitPos++ % 8)) & 1;
		};
		bool HasNext()
		{
			//i dont know if i'm bad at math or this universe is fooling me... <+1> shouldn't be here...
			//but we are protected from going out of bounds by Decode() in ShannonFanoEncoder
			//so it doesn't matter
			return (currentBitPos < str->length * 8 + 1);
		};

		ShannonFanoEncodedStringBitIterator(String* str)
		{
			this->str = str;
			currentBitPos = countByteSize * 8;
		}
	};
	ShannonFanoEncodedStringBitIterator* CreateBitIterator()
	{
		return new ShannonFanoEncodedStringBitIterator(this);
	}

	void WriteShannonFanoBits(List<bool>& bits, size_t letterCount)
	{
		delete value;

		//save letterCount for decoding purposes...
		size_t bytesForLetterCount = 8;


		size_t charCount = bits.Count() / 8 + bytesForLetterCount;
		if (bits.Count() % 8 != 0)
			charCount++;

		length = charCount;
		value = new char[length + 1];


		//cout << "\nencoding " << letterCount << " to 8 bytes...\n";
		
		//now we smash letterCount across chars
		//hell yeah
		for (size_t i = 0; i < bytesForLetterCount; i++)
			value[i] = letterCount >> (8 - 1 - i) * 8;

		//init writable chars
		for (size_t i = bytesForLetterCount; i < length; i++)
			value[i] = 0;

		value[length] = '\0';

		//now we smash these bits across chars
		//hell yeah
		for (size_t c = bytesForLetterCount; c < length; c++)
		{
			size_t offset = c * 8;
			for (size_t bit = 0; bit < 8; bit++)
			{
				size_t bitListIndex = offset + bit - bytesForLetterCount * 8;
				if (bitListIndex > bits.Count() - 1)
					break;

				if (bits[bitListIndex])
					value[c] |= 1 << bit;
				else
					value[c] &= ~(1 << bit);
			}
		}

	}
};