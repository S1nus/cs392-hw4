#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

struct blockinfo {
	int firstRow, firstCol, insertRow, insertCol;
};

struct blockinfo blockNums(int matrixWidth, int blockWidth, int blockNum, float* matrix) {
	struct blockinfo result;
	int blocksperrow = matrixWidth / blockWidth;

	int row = (blockNum/blocksperrow) * blockWidth;
	result.firstRow = row;

	int col = blockWidth*(blockNum % blocksperrow);
	result.firstCol = col;

	result.insertCol = row;
	result.insertRow = col;

	return result;
}

float* transposeBlock(int matrixWidth, int blockWidth, int blockNum, float* matrix) {

	float* block = (float*) malloc(blockWidth*blockWidth*sizeof(float));
	float* transposed = (float*) malloc(blockWidth*blockWidth*sizeof(float));
	int blocksperrow = matrixWidth / blockWidth;

	int row = (blockNum/blocksperrow) * blockWidth;
	int col = blockWidth*(blockNum % blocksperrow);
	int place = 0;

	for (int i = row; i<(row+blockWidth); i++) {
		for (int j = col; j<(col+blockWidth); j++) {
			block[place] = matrix[i*matrixWidth + j];
			place++;
		}
	}

	for (int row = 0; row < blockWidth; row++) {
		for (int col = 0; col < blockWidth; col++) {
			transposed[row*blockWidth + col] = block[col*blockWidth + row];
		}
	}
	free(block);
	return transposed;
}
int main(int argc, char** argv) {
	if (argc != 3) {
		printf("This program requries two arguments: the width of the input matrix, and the width of the block\n");
		return (-1);
	}

	int matrixWidth = atoi(argv[1]);
	int blockWidth = atoi(argv[2]);

	int numBlocks = (matrixWidth*matrixWidth)/(blockWidth*blockWidth);

	float* matrix = (float*) malloc(sizeof(float) * matrixWidth*matrixWidth);
	float* transposedMatrix = (float*) malloc(sizeof(float) * matrixWidth*matrixWidth);

	for (int i = 0; i < (matrixWidth * matrixWidth); i++) {
		float max = 100;
		float randNum = ( (float) rand() / (float) (RAND_MAX) ) * max;
		matrix[i] = randNum;
	}

	/*printf("Untransposed:\n");
	for (int i = 0; i < matrixWidth; i++) {
		for (int j = 0; j < matrixWidth; j++) {
			printf("%.1f ", matrix[i*matrixWidth + j]);
		}
		printf("\n");
	}*/

	struct timeval t1;
	gettimeofday(&t1, NULL);
	//long double usec = ((long double) t1.tv_sec * 1000) + (long double) t1.tv_usec;
	printf("\ncurrent time: %ld.%06ld\n", t1.tv_sec, t1.tv_usec);
	//printf("\nusec: %ld\n", usec); 
	for (int blocknum = 0; blocknum < numBlocks; blocknum++) {

		float* block = transposeBlock(matrixWidth, blockWidth, blocknum, matrix);
		struct blockinfo blockinfo = blockNums(matrixWidth, blockWidth, blocknum, matrix);
		for (int i = 0; i<blockWidth; i++) {
			for (int j = 0; j<blockWidth; j++) {
				transposedMatrix[(blockinfo.insertRow+i)*matrixWidth + (blockinfo.insertCol+j)] = block[i*blockWidth + j];	
			}
		}

		free(block);
	}
	struct timeval t2;
	gettimeofday(&t2, NULL);
	printf("\ncurrent time: %ld.%06ld\n", t2.tv_sec, t2.tv_usec);
	time_t diff = ((t1.tv_sec * 1000000)+t1.tv_usec - (t2.tv_sec*1000000)+t2.tv_usec);
	printf("\ndiff: %06ld useconds\n", diff);

	/*printf("\nTransposed:\n");
	for (int row = 0; row < matrixWidth; row++) {
		for (int col = 0; col < matrixWidth; col++) {
			printf("%.1f ", transposedMatrix[row*matrixWidth + col]);
		}
		printf("\n");
	}*/
}
