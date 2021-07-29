#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void zeroDigger(int** m_f, int** p_f, int m_x, int m_y, int f_x, int f_y) {
	int x2, y2;
	for (x2 = m_x - 1; x2<m_x + 2; x2++) {
		for (y2 = m_y - 1; y2<m_y + 2; y2++) {
			if (!(x2<0 || y2<0 || x2>f_x - 1 || y2>f_y - 1)) {
				if (m_f[y2][x2] != 9) {
					if (p_f[y2][x2] == 0) {
						p_f[y2][x2] = 1;
						if (m_f[y2][x2] == 0) {
							zeroDigger(m_f, p_f, x2, y2, f_x, f_y);
						}
					}
				}
			}
		}
	}
}

int mineGenerator(int** field, int w, int h, int mineNum) {
	int i, j;
	for (i = 0; i<h; i++) {
		for (j = 0; j<w; j++) {
			field[i][j] = 0;
		}
	}
	srand(time(NULL));
	for (i = 0; i<mineNum; i++) {
		int x = rand() % w;
		int y = rand() % h;
		if (field[y][x] == 9) {
			--i;
			continue;

		}
		field[y][x] = 9;
		int x2, y2;
		for (x2 = x - 1; x2<x + 2; x2++) {
			for (y2 = y - 1; y2<y + 2; y2++) {
				if ((x2 == x&&y2 == y) || (x2<0 || y2<0 || x2>w - 1 || y2>h - 1) || field[y2][x2] == 9) {

				}
				else {
					field[y2][x2]++;
				}
			}
		}
	}
	return i;
}

void minePrinter(int** field, int w, int h) {
	int x, y;
	for (y = 0; y<h; y++) {
		for (x = 0; x<w; x++) {
			printf("%d ", field[y][x]);
		}
		printf("\n");
	}
}


void playerFieldPrinter(int** mf, int** pf, int w, int h) {
	int x, y, z;
	z = 0;
	printf("0 1 2 3 4 5 6 7 8 9    x/y\n\n");
	for (y = 0; y<h; y++) {
		for (x = 0; x <= w; x++) {
			if (pf[y][x] == 0) {
				printf("C ");
			}
			else if (pf[y][x] == 1) {
				printf("%d ", mf[y][x]);
			}
			else if (pf[y][x] == 2) {
				printf("F ");
			}
			else if (pf[y][x] == 3)
			{
				printf("? ");
			}
			else if (x == w) {
				printf("   %d", z++);
			}
		}
		printf("\n");
	}
}

int main() {
	int m_X, m_Y, mineNum;
	int** mf;
	int** pf;
	int i, j;
	time_t t1, t2;
	int clearCount;
	printf("Size of minesweeper (doesn't work occationally, try again if it doens't work)\nx, y:");
	scanf_s("%d, %d", &m_X, &m_Y);
	printf("Number of mines :");
	scanf_s("%d", &mineNum);

	clearCount = (m_X * m_Y) - mineNum;
	printf("clearcount : %d\n", clearCount);


	mf = (int**)malloc(sizeof(int*)*m_X);
	pf = (int**)malloc(sizeof(int*)*m_X);
	for (i = 0; i<m_X; i++){
		mf[i] = (int*)malloc(sizeof(int)*m_Y);
		pf[i] = (int*)malloc(sizeof(int)*m_Y);
	}

	for (i = 0; i<m_X; i++) {
		for (j = 0; j<m_Y; j++) {
			pf[j][i] = 0;
		}
	}

	mineGenerator(mf, m_X, m_Y, mineNum);
	time(&t1);
	while (1) {
		char input;
		printf("(q)uit, (d)dig, (f)lag or (?):");
		while (1) {
			scanf_s("%c", &input);
			if (input == 'q'){
				return 0;
			}
			else if (input == 'd'){
				int x, y;
				printf("where to dig? :\n(y means the y axis, x means the x axis, sorry for the inconvenience)\ny, x: ");
				scanf_s("%d, %d", &x, &y);
				if (mf[y][x] == 9){
					minePrinter(mf, m_X, m_Y);

					printf("Game Over!\n");
					
					printf("\a\a\a\a\a\a");
					return 0;
				}
				else{
					//zeroDigger(mf,pf,x,y,m_X,m_Y);
					pf[y][x] = 1;
					if (mf[y][x] == 0) {
						zeroDigger(mf, pf, x, y, m_X, m_Y);
					}
					playerFieldPrinter(mf, pf, m_X, m_Y);
					for (i = 0; i<m_X; i++) {
						for (j = 0; j<m_Y; j++){
							if (pf[j][i] == 1) {
								clearCount--;
							}
						}
					}
					printf("clearcount2 : %d\n", clearCount);
					if (clearCount == 0) {
						time(&t2);
						printf("%ld(sec)", t2 - t1);
						printf("game clear\n");
					
						return 0;
					}
					else {
						clearCount = (m_X * m_Y) - mineNum;
					}
					break;
				}
			}
			else if (input == 'f'){
				int x, y;
				printf("where to flag? : \n(y means the y axis, x means the x axis, sorry for the inconvenience)\ny, x: ");
				scanf_s("%d, %d", &x, &y);
				if (pf[y][x] == 1){
					printf("Already Digged\n");
					break;
				}
				else{
					pf[y][x] = 2;
				}
				playerFieldPrinter(mf, pf, m_X, m_Y);
				break;
			}

			else if (input == '?'){
				int x, y;
				printf("where to '?' ?");
				scanf_s("%d, %d", &x, &y);

				if (pf[y][x] == 1){
					printf("Already Digged\n");
				}
				else{ pf[y][x] = 3; }
				playerFieldPrinter(mf, pf, m_X, m_Y);
				break;
			}

			else if (input == '\n'){

			}
			else{
				printf("Wrong input. ");
				break;
			}
		}
	}

	return 0;
}




