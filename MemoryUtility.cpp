//MemoryUtility.cpp - Contains definitions for memory management / memory access utility classes.
#include "MemoryUtility.h"

/*
template <class elementType>
CStatic2DArray<elementType>::CStatic2DArray(const MemIndexType elementsPerRow,
		const MemIndexType elementsPerColumn)
{
	if((elementsPerRow > 0) && (elementsPerColumn > 0)) {
		//Allocate memory for the array representation.
		this->mpArrayData = new elementType[elementsPerRow][elementsPerColumn];
	}
}

template <class elementType>
CStatic2DArray<elementType>::~CStatic2DArray()
{
	if (this->mArrayData) {
		//Free the memory allocated for the two-dimensional array.
		delete [] this->mpArrayData;
	}
}


//Determines the validity of the represented two-dimensional storage stucture.
template <class elementType>
bool CStatic2DArray<elementType>::IsValid() const
{
	return(this->mpArrayData && (this->m_RowSize > 0) && (this->m_ColumnSize > 0));
}

template <class elementType>
const elementType* CStatic2DArray<elementType>::GetElementAt(MemIndexType xIndex,
		MemIndexType yIndex) const
{
	elementType*					pElementPointer = NULL;

	if (this->IsValid() && this->mpArrayData &&
		(xIndex < this->mRowsize) && (yIndex < this->mColumnSize))
	{
		//Retrieve a pointer to the data at the specified index.
		pElementPointer = &this->mpArrayData[xIndex][yIndex];		
	}

	return(pElementPointer);
}

template <class elementType>
bool CStatic2DArray<elementType>::SetElementAt(const MemIndexType xIndex,
											  const MemIndexType yIndex,
											  const elementType& element)
{
	bool							bElementSuccessfullySet = false;

	//Assign the element to the position in the array. Represented
	//classes may require a copy operator.
	if (this->IsValid() && mpArrayData) {
		this->mpArrayData[xIndex][yIndex] = element;
	}

	return(bElementSuccessfullySet);
}
*/