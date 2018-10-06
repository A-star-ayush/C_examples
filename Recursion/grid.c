#include <stdio.h>

int r, c;  // number of rows and columns

int findPath(int grid[r][c], int marked[r][c], char path[r][c], int len, int x, int y, int desX, int desY){
	if(x<0 || y<0 || x>=r || y>=c)
		return 0;
	
	if(grid[x][y]==1 || marked[x][y]==1)
		return 0;

	path[x][y]='#';
	marked[x][y]=1;

	if(x==desX && y==desY)
		return 1;
	
	else {
		return findPath(grid, marked, path, len+1, x, y+1, desX, desY) ||
			   findPath(grid, marked, path, len+1, x, y-1, desX, desY) ||
			   findPath(grid, marked, path, len+1, x+1, y, desX, desY) ||
			   findPath(grid, marked, path, len+1, x-1, y, desX, desY) ;
	}

}

int main(int argc, char const *argv[])
{
	printf("Rows: ");
	scanf("%d",&r);

	printf("Cols: ");
	scanf("%d",&c);

	int grid[r][c];

	printf("Enter the elements of the grid: \n");

	int i,j;

	for(i=0;i<r;i++){
		for(j=0;j<c;j++)
			scanf("%d",&grid[i][j]);
	}

	int marked[r][c];

	for(i=0;i<r;i++){
		for(j=0;j<c;j++)
			marked[i][j]=0;
	}


	char path[r][c];
	for(i=0;i<r;i++){
		for(j=0;j<c;j++){
			if(grid[i][j])
				path[i][j]='1';
			else
				path[i][j]='0';
		}
	}

	__fpurge(stdin);
	printf("Enter source (x,y): ");
	int x1,y1;
	scanf("(%d,%d)",&x1, &y1);
	
	__fpurge(stdin);

	printf("Enter destination (x,y): ");
	int x2,y2;
	scanf("(%d,%d)",&x2, &y2);

	if(findPath(grid, marked, path, 0, x1-1, y1-1, x2-1, y2-1)){
		puts("A path exists:");
		for(i=0;i<r;i++){
			for(j=0;j<c;j++)
				printf("%c ",path[i][j]);
			puts("");
		}
	}

	else
		puts("No path exists");
	
	return 0;
}