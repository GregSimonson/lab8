#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
    if (l < r) {
        int z = l + (r - l) / 2; // Find the middle index

        // Sorts the first and second halves
        mergeSort(pData, l, z);
        mergeSort(pData, z + 1, r);

        // Allocates the  memory for temp arrays left and right
        int num1 = z - l + 1;
        int num2 = r - z;
        int *left = (int *)Alloc(num1 * sizeof(int));
        int *right = (int *)Alloc(num2 * sizeof(int));

        // Copoies the data to temporary arrays left[i] and right[i]
        memcpy(left, &pData[l], num1 * sizeof(int));
        memcpy(right, &pData[z + 1], num2 * sizeof(int));

        // Merges the sorted halves
        int i = 0, j = 0, k = l;

        // Merges the two arrays
        while (i < num1 && j < num2) {
            if (left[i] <= right[j]) {
                pData[k] = left[i];
                i++;
            } else {
                pData[k] = right[j];
                j++;
            }
            k++;
        }

        // Copies the remaining elements of left[i] if any
        while (i < num1) {
            pData[k] = left[i];
            i++;
            k++;
        }

        // Copy the remaining elements of right[i] if any
        while (j < num2) {
            pData[k] = right[j];
            j++;
            k++;
        }

        // Deallocate memory for temporary arrays
        DeAlloc(left);
        DeAlloc(right);
    }
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
    printf("\tData:\n\t");

    if (dataSz <= 100) {
        for (int i = 0; i < dataSz; ++i) {
            printf("%d ", pData[i]);
        }
    } else {
        for (int i = 0; i < 100; ++i) {
            printf("%d ", pData[i]);
        }
        printf("\n\t\n\t");
        for (int i = dataSz - 100; i < dataSz; ++i) {
            printf("%d ", pData[i]);
        }
    }

    printf("\n\n");
}


int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}