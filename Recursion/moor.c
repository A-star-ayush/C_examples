#include <stdio.h>

// A moor consists of land zones (which can be passed) and water zones (which cannot be passed)
// The moor traversal problem is to find whether a path exists form col first to last of adjacent land zones
// and in every move one must move one col to the right

// Here 1 represents a land zone and 0 represents a water zone

int n,m;

int findPath(int arr[n][m], int marked[n][m], int x, int y, int path[m]){
	if(x<0 || y<0 || x>=n || y>=m)  // boundary conditions  [make sure not to miss out on them]
		return 0;
	
	if(arr[x][y]==0 || marked[x][y])  // encountered a water zone in the way
		return 0;   
	
	path[y]=x;  // just the row numbers are sufficient since we are advancing by a col in each turn
	marked[x][y]=1;   // by using the marked array we make sure we do not visit the same path again
												// very important for efficiency and for avoiding infinte loop
	if(y==m-1)
		return 1;  // made it to the last col 
	else
	{			
		return  findPath(arr, marked, x-1, y+1, path) ||   // upper right diagonal
				findPath(arr, marked, x, y+1, path) ||    // right
				findPath(arr, marked, x+1, y+1, path);   // lower right diagonal
	}			// """ NOTE: each time we try n make a move to the next col """
}			// Such a movement is due to the nature of the problem and is not at all generic
							// More generally we consider moving left, right, up and down

int main(int argc, char const *argv[])
{
	printf("Enter the dimensions: ");
	scanf("%d %*[xX-] %d",&n,&m);  // skips over any number of X's, x's and -'s if present btw n and m 	
	int arr[n][m];							// but also makes it compulsory to type atleast one of them
	puts("Provide the elements of the moor [1- land zone; 0- water zone]:");
	__fpurge(stdin);  // to remove any unwanted input before inputting the moor 
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
			scanf("%d", &arr[i][j]);

	int path[m];  // we exploit the fact that for this particular problem the path is of fixed length
	printf("\n");
	
	int marked[n][m];  // a variable sized array cannot be directly initialized
	for(i=0;i<n;i++){
		for(j=0;j<m;j++)
			marked[i][j]=0;
	}

	for(i=0;i<n;i++){
		if(arr[i][0]==1){
		if(findPath(arr,marked,i,0, path)){
			printf("Found a path starting from (%d,0).\n",i);
			for(j=0;j<m;j++)
				printf("%d ",path[j]);
			puts("");
			return 0;
		}
	   }
	}

		puts("Could not find a path.\n");
	return 0;
}