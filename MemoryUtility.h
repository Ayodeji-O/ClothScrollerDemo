//MemoryUtility.h - Contains declarations for emory management / memory access utility classes.

#ifndef _MEMORYUTILITY_H
#define _MEMORYUTILITY_H			1

#include "stdio.h"
#include "stdlib.h"

#define MemIndexType unsigned long
#define MemSizeType	 unsigned long


template <class elementType> class CStatic2DArray
{
public:
	CStatic2DArray(const MemIndexType elementsPerRow,
		const MemIndexType elementsPerColumn);
	~CStatic2DArray();

	//Determines the validity of the array.
	bool							IsValid() const;

	//Retrieves a pointer to the element at the specified
	//index pair.
	elementType*					GetElementAt(const MemIndexType xIndex,
		const MemIndexType yIndex) const;

	//Sets the element at the specified location in the array to a particular
	//value.
	bool							SetElementAt(const MemIndexType xIndex,
		const MemIndexType yIndex, const elementType& element);

	//Returns the number of elements per table row.
	MemIndexType					GetRowSize() const;

	//Returns the number of elements per table column.
	MemIndexType					GetColumnSize() const;
private:
	//Allocates memory required for the array.
	bool							AllocateArrayMemory(const MemIndexType elementsPerRow,
		const MemIndexType elementsPerColumn);
	//De-allocates memory that has been allocated for the array.
	bool							DeAllocateArrayMemory();

	//Number of elements per row ("width" of two-dimensional array).
	const MemIndexType				mRowSize;
	//Number of elements per column ("height" of two-dimensional array).
	const MemIndexType				mColumnSize;

	//Array data (created at class instantiation time).
	elementType**					mpArrayData;
};

/////
// CStatic2DArray
/////

// Template class - implementation must exist in header file.
template <class elementType>
CStatic2DArray<elementType>::CStatic2DArray(const MemIndexType elementsPerRow,
											const MemIndexType elementsPerColumn) : mRowSize(elementsPerRow),
											mColumnSize(elementsPerColumn), mpArrayData(NULL)
{
	//Allocate memory required for the two-dimensional array.
	::AssertExpression(this->AllocateArrayMemory(elementsPerRow, elementsPerColumn));
}

template <class elementType>
CStatic2DArray<elementType>::~CStatic2DArray()
{
	//Free the memory that has been allocated for the array.
	this->DeAllocateArrayMemory();
}


//Determines the validity of the represented two-dimensional storage stucture.
template <class elementType>
bool CStatic2DArray<elementType>::IsValid() const
{
	return(this->mpArrayData && (this->mRowSize > 0) && (this->mColumnSize > 0));
}

template <class elementType>
elementType* CStatic2DArray<elementType>::GetElementAt(MemIndexType xIndex,
		MemIndexType yIndex) const
{
	elementType*					pElementPointer = NULL;

	if (this->IsValid() && this->mpArrayData &&
		(xIndex < this->GetRowSize()) && (yIndex < this->GetColumnSize()))
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
	if (this->IsValid() && mpArrayData &&
		(xIndex < this->GetRowSize()) && (yIndex < this->GetColumnSize()))
	{
		(this->mpArrayData[xIndex][yIndex]) = element;

		bElementSuccessfullySet = true;
	}

	return(bElementSuccessfullySet);
}

template <class elementType>
MemIndexType CStatic2DArray<elementType>::GetRowSize() const
{
	MemIndexType					rowSize = 0;

	//If the array has been properly initialized, return the number
	//of elements allocated per row.
	if(this->IsValid()) {
		rowSize = this->mRowSize;
	}

	return(rowSize);
}

template <class elementType>
MemIndexType CStatic2DArray<elementType>::GetColumnSize() const
{
	MemIndexType					columnSize = 0;

	//If the array has been properly initialized, return the number
	//of elements allocated per column.
	if(this->IsValid()) {
		columnSize = this->mColumnSize;
	}


	return(columnSize);
}

template <class elementType>
bool CStatic2DArray<elementType>::AllocateArrayMemory(const MemIndexType elementsPerRow,
		const MemIndexType elementsPerColumn)
{
	bool							bAllocatedSuccessfully = false;

	if((elementsPerRow > 0) && (elementsPerColumn > 0) && !this->mpArrayData) {
		//Allocate memory for the array representation.
		this->mpArrayData = new elementType*[elementsPerRow];

		if(::AssertExpression(this->mpArrayData != NULL)) {
			for(MemIndexType rowLoop = 0; rowLoop < elementsPerRow; rowLoop++) {
				this->mpArrayData[rowLoop] = new elementType[elementsPerColumn];
			}
		}

		bAllocatedSuccessfully = true;
	}

	return(bAllocatedSuccessfully);
}

template <class elementType>
bool CStatic2DArray<elementType>::DeAllocateArrayMemory()
{
	bool							bDeallocatedSuccessfully = false;

	if (this->mpArrayData) {

		const MemIndexType			elementsPerRow = this->GetRowSize();

		for(MemIndexType rowLoop = 0; rowLoop < elementsPerRow; rowLoop++) {
			//Free the memory allocated for all "columns" in the two-dimensional
			//array.
			delete [] this->mpArrayData[rowLoop];
		}

		//Free the memory allocated for all "rows" in the two-dimensional
		//array.
		delete [] this->mpArrayData;

		bDeallocatedSuccessfully = true;
	}

	return(bDeallocatedSuccessfully);
}

#endif	//#ifndef _MEMORYUTILITY_H