// Copyright Apostol Rares-Andrei 311CAa 2022-2023
#include<stdio.h>
#include<stdlib.h>

#define N_MODULO 10007

int **free_matrix(int **a, int m);
int *free_array(int *v);

// eliberez toate resursele alocate
void free_all(int ***p, int *rows, int *colls, int *sum, int num)
{
	if (num > 0) {
		for (int i = 0; i < num; i++)
			p[i] = free_matrix(p[i], rows[i]);
		rows = free_array(rows);
		colls = free_array(colls);
		sum = free_array(sum);
		free(p);
	}
}

void swap_int(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

void swap_pptr(int ***a, int ***b)
{
	int **aux = *a;
	*a = *b;
	*b = aux;
}

int *free_array(int *v)
{
	free(v);
	v = NULL;
	return v;
}

// eliberez o matrice
int **free_matrix(int **a, int m)
{
	for (int i = 0; i < m; i++)
		free_array(a[i]);
	free(a);
	a = NULL;
	return a;
}

// aloc o matrice
int **alloc_matrix(int **a, int m, int n)
{
	a = (int **)malloc(m * sizeof(int *));

	if (!a) {
		printf("Failed to allocate memory\n");
		return NULL;
	}

	for (int i = 0; i < m; i++) {
		a[i] = (int *)malloc(n * sizeof(int));
		if (!a[i]) {
			printf("Failed to allocate memory\n");
			return NULL;
		}
	}

	return a;
}

// citesc(si aloc) o matrice
int **read_matrix(int **a, int m, int n)
{
	a = alloc_matrix(a, m, n);
	if (!a)
		return NULL;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			scanf("%d", &a[i][j]);
	}

	return a;
}

// aloc spatiu un plus la un ***p.
int ***alloc_ppptr(int ***v, int num)
{
	/* daca num este 0 aloc spatiu pentru o singura adresa,
	altfel maresc dimensiunea cu 1 */

	if (num == 0)
		v = (int ***)malloc(sizeof(int **));
	else
		v = (int ***)realloc(v, (num + 1) * sizeof(int **));

	if (!v) {
		printf("Failed to allocate memory\n");
		// direct free la tot p_matrix
		return NULL;
	}
	return v;
}

// aloc spatiu in plus la un *p.
int *alloc_ptr(int *v, int num)
{
	/* daca num este 0 aloc spatiu pentru o singura adresa,
	altfel maresc dimensiunea cu 1 */
	if (num == 0)
		v = (int *)malloc(sizeof(int));
	else
		v = (int *)realloc(v, (num + 1) * sizeof(int));

	if (!v) {
		printf("Failed to allocate memory\n");
		return NULL;
	}
	return v;
}

/* adaug dimensiunile matricei noi la finalul vectorilor
 care pastreaza dimensiunile */
void update_size(int *rows, int *colls, int m, int n, int *num)
{
	rows[*num] = m;
	colls[*num] = n;
	(*num)++;
}

/* verific daca s-a putut aloca o matrice
 in caz contrar, eliberez toate resursele */
void check_matrix(int **a, int *rows, int *colls, int *sum, int ***p, int num)
{
	if (!a) {
		printf("Failed to allocate memory\n");
		free_all(p, rows, colls, sum, num);
		exit(1);
	}
}

/* verific daca s-au putut face alocarile
 in caz contrar, eliberez toate resursele */
void check_allocs(int *rows, int *colls, int *sum, int ***p, int num)
{
	if (!rows || !colls || !sum || !p) {
		printf("Failed to allocate memory\n");
		free_all(p, rows, colls, sum, num);
		exit(1);
	}
}

// afisez o matrice de la un indice primit
void print_matrix(int ***p, int *rows, int *colls, int num)
{
	int index;
	scanf("%d", &index);
	if (index >= num || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		int **a = p[index];
		for (int i = 0; i < rows[index]; i++) {
			for (int j = 0; j < colls[index]; j++)
				printf("%d ", a[i][j]);
			printf("\n");
		}
	}
}

// afisez dimensiunile unei matrici
void print_matrix_size(int *rows, int *colls, int num)
{
	int index;
	scanf("%d", &index);
	if (index >= num || index < 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", rows[index], colls[index]);
}

// operatia de redimensionare a unei matrici
int **resize_matrix(int **a, int *rows, int *colls, int *index)
{
	int old_m = rows[*index];

	int *new_lines = NULL;
	int *new_columns = NULL;
	int m, n;

	// aloc memorie pentru datele citite

	scanf("%d", &m);
	new_lines = alloc_ptr(new_lines, m);
	for (int i = 0; i < m; i++)
		scanf("%d", &new_lines[i]);

	scanf("%d", &n);
	new_columns = alloc_ptr(new_columns, n);
	for (int i = 0; i < n; i++)
		scanf("%d", &new_columns[i]);

	// aloc memorie pentru matricea noua
	int **new_matrix = NULL;
	new_matrix = alloc_matrix(new_matrix, m, n);

	if (!new_matrix || !new_columns)
		return NULL;
	if (!new_lines)
		return NULL;

	// actualizez dimensiunile
	rows[*index] = m;
	colls[*index] = n;

	// pun rezulatul in matricea noua
	for (int i = 0; i < m; i++) {
		int x = new_lines[i];
		for (int j = 0; j < n; j++) {
			int y = new_columns[j];
			new_matrix[i][j] = a[x][y];
		}
	}

	a = free_matrix(a, old_m);
	a = alloc_matrix(a, m, n);
	if (!a)
		return NULL;

	// copiez valorile in a

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i][j] = new_matrix[i][j];
	}

	// dealoc memoria folosita

	new_columns = free_array(new_columns);
	new_lines = free_array(new_lines);
	new_matrix = free_matrix(new_matrix, m);

	return a;
}

// verific daca se poate face redimensionarea
void check_resize(int ***p, int *rows, int *colls, int *index, int num, int *ok)
{
	scanf("%d", index);
	if (*index >= num || *index < 0) {
		// chiar daca indexul nu e bun, citesc in continuare
		int aux, x;
		scanf("%d", &aux);
		for (int i = 0; i < aux; i++)
			scanf("%d", &x);
		scanf("%d", &aux);
		for (int i = 0; i < aux; i++)
			scanf("%d", &x);
		printf("No matrix with the given index\n");
		*ok = 0;
	} else {
		int **a = p[*index];
		p[*index] = resize_matrix(a, rows, colls, index);
		*ok = 1;
	}
}

// produsul dintre matrici
int **product(int **a, int **b, int *rows, int *colls, int fir, int sec)
{
	int new_m = rows[fir];
	int new_n = colls[sec];
	int **prod = NULL;
	prod = alloc_matrix(prod, new_m, new_n);
	if (!prod)
		return NULL;

	long long copy;

	for (int i = 0; i < rows[fir]; i++) {
		for (int j = 0; j < colls[sec]; j++) {
			copy = 0;
			for (int k = 0; k < rows[sec]; k++)
				copy += a[i][k] * b[k][j];
			copy %= N_MODULO;
			if (copy < 0)
				copy += N_MODULO;
			prod[i][j] = copy;
		}
	}
	return prod;
}

// verific daca pot face produsul
int check_prod(int *f, int *s, int *colls, int *rows, int n)
{
	scanf("%d%d", f, s);
	if (*f >= n || *f < 0) {
		printf("No matrix with the given index\n");
		return 0;
	} else if (*s >= n || *s < 0) {
		printf("No matrix with the given index\n");
		return 0;
	} else if (colls[*f] != rows[*s]) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	}
	return 1;
}

/* verific alocarile si pregatesc variabilele necesare pentru
 a face produsul */
void prod_matrix(int f, int s, int ***p, int *rows, int *coll, int *sum, int *n)
{
	check_allocs(rows, coll, sum, p, *n);
	int **a = p[f];
	int **b = p[s];
	rows[*n] = rows[f];
	coll[*n] = coll[s];
	p[*n] = product(a, b, rows, coll, f, s);
	check_matrix(p[*n], rows, coll, sum, p, *n);
	(*n)++;
}

// calculez suma pentru fiecare matrice
void sum_matrix(int ***p_matrix, int *sum, int num, int *rows, int *colls)
{
	if (num > 0) {
		long long big_sum;
		for (int i = 0; i < num; i++) {
			int **a = p_matrix[i];
			big_sum = 0;
			for (int j = 0; j < rows[i]; j++) {
				for (int k = 0; k < colls[i]; k++)
					big_sum += a[j][k];
				sum[i] = big_sum % N_MODULO;
				if (sum[i] < 0)
					sum[i] += N_MODULO;
			}
		}
	}
}

// sortez crescator matricile
void sort_matrix(int ***p_matrix, int *sum, int *rows, int *colls, int num)
{
	if (num <= 0)
		return;
	for (int i = 0; i < num - 1; i++) {
		for (int j = i + 1; j < num; j++) {
			if (sum[i] > sum[j]) {
				swap_int(&sum[i], &sum[j]);
				swap_int(&rows[i], &rows[j]);
				swap_int(&colls[i], &colls[j]);
				swap_pptr(&p_matrix[i], &p_matrix[j]);
			}
		}
	}
}

// operatia de transpunere
int **transp_matrix(int **a, int m, int n)
{
	int **new_matrix = NULL;
	int new_m = n;
	int new_n = m;

	// aloc noua matrice
	new_matrix = alloc_matrix(new_matrix, new_m, new_n);

	if (!new_matrix) {
		printf("Failed to allocate memory\n");
		return NULL;
	}

	for (int i = 0; i < new_n; i++) {
		for (int j = 0; j < new_m; j++)
			new_matrix[j][i] = a[i][j];
	}

	// realoc matricea pe noile dimensiuni
	a = free_matrix(a, m);
	a = NULL;
	a = alloc_matrix(a, new_m, new_n);

	if (!a) {
		printf("Failed to allocate memory\n");
		return NULL;
	}

	// copiez matricea noua in a
	for (int i = 0; i < new_m; i++) {
		for (int j = 0; j < new_n; j++)
			a[i][j] = new_matrix[i][j];
	}

	// eliberez memoria folosita
	new_matrix = free_matrix(new_matrix, new_m);
	return a;
}

// verific daca pot face transpusa
void check_transp(int ***p_matrix, int *rows, int *colls, int num, int *sum)
{
	int index;
	scanf("%d", &index);
	if (index < num && index >= 0) {
		int old_m = rows[index];
		int old_n = colls[index];
		int **a = p_matrix[index];
		p_matrix[index] = transp_matrix(a, old_m, old_n);
		check_matrix(p_matrix[index], rows, colls, sum, p_matrix, num);
		swap_int(&colls[index], &rows[index]);
	} else {
		printf("No matrix with the given index\n");
	}
}

/* initializez matricea identitate cu 1 pe diagonala
 si 0 in rest */
void initialize_I_matrix(int **I, int size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j)
				I[i][j] = 1;
			else
				I[i][j] = 0;
		}
	}
}

// produsul pentru operatia de ridicare la putere
int **pwr_product(int **a, int **b, int size)
{
	int **aux = NULL;
	aux = alloc_matrix(aux, size, size);
	if (!aux)
		return NULL;

	long long copy;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			copy = 0;
			for (int k = 0; k < size; k++)
				copy += a[i][k] * b[k][j];
			copy %= N_MODULO;
			if (copy < 0)
				copy += N_MODULO;
			aux[i][j] = copy;
		}
	}
	// copiez valorile in a
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			a[i][j] = aux[i][j];

	// eliberez memoria folosita
	aux = free_matrix(aux, size);
	return a;
}

// ridicare la putere in timp logaritmic
int **matrix_power(int **a, int **I, int size, int pow)
{
	int **aux = NULL;
	aux = alloc_matrix(aux, size, size);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			aux[i][j] = I[i][j];

	while (pow > 1) {
		if (pow % 2 == 0) {
			a = pwr_product(a, a, size);
			pow /= 2;
		} else {
			aux = pwr_product(aux, a, size);
			a = pwr_product(a, a, size);
			pow = (pow - 1) / 2;
		}
	}

	a = pwr_product(a, aux, size);
	aux = free_matrix(aux, size);
	return a;
}

// verific daca se poate ridica o matrice la putere
void check_pow_matrix(int ***p, int *rows, int *colls, int num, int *sum)
{
	int index, pow;
	scanf("%d%d", &index, &pow);
	if (index >= num || index < 0) {
		printf("No matrix with the given index\n");
	} else if (pow < 0) {
		printf("Power should be positive\n");
	} else if (rows[index] != colls[index]) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		int **a = p[index];
		int size = rows[index];
		int **I =  NULL;
		I = alloc_matrix(I, size, size);
		check_matrix(I, rows, colls, sum, p, num);
		initialize_I_matrix(I, size);
		if (pow == 0) {
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					a[i][j] = I[i][j];
			I = free_matrix(I, size);
		} else {
			p[index] = matrix_power(a, I, size, pow);
			check_matrix(p[index], rows, colls, sum, p, num);
			I = free_matrix(I, size);
		}
	}
}

// mut o matrice pe ultima pozitie din vector pentru a o elibera
void move_last(int ***ptr, int *rws, int *coll, int *sum, int indx, int n)
{
	for (int i = indx; i < n - 1; i++) {
		swap_int(&rws[i + 1], &rws[i]);
		swap_int(&coll[i + 1], &coll[i]);
		swap_int(&sum[i + 1], &sum[i]);
		swap_pptr(&ptr[i + 1], &ptr[i]);
	}
}

// verific daca se poate face eliberarea
void check_free(int ***p, int *rows, int *colls, int *sum, int num, int *ok)
{
	*ok = 0;
	int index;
	scanf("%d", &index);
	if (index >= num || index < 0) {
		printf("No matrix with the given index\n");
		*ok = 0;
	} else {
		move_last(p, rows, colls, sum, index, num);
		if (num == 1) {
			// daca este incarcata o singura matrice, eliberez toate resursele
			free_all(p, rows, colls, sum, num);
			num = 0;
			p = NULL;
			rows = NULL;
			colls = NULL;
			sum = NULL;
			*ok = 1;
		} else if (num > 1) {
			int **a = p[num - 1];
			int m = rows[num - 1];
			p[num - 1] = free_matrix(a, m);
			*ok = 2;
		}
	}
}

// actualizez dimensiunile pentru tablourile alocate
void resize_all(int ****p, int **colls, int **rows, int **sum, int num)
{
	*p = alloc_ppptr(*p, num);
	*colls = alloc_ptr(*colls, num);
	*rows = alloc_ptr(*rows, num);
	*sum = alloc_ptr(*sum, num);
}

// impart o matrice dupa cei doi indici de la care incepe
int **split_matrix(int **a, int size, int m, int n)
{
	int new_size = size / 2, **new_matrix = NULL;
	new_matrix = alloc_matrix(new_matrix, new_size, new_size);

	for (int i = 0; i < new_size; i++) {
		int c = n;
		for (int j = 0; j < new_size; j++) {
			new_matrix[i][j] = a[m][c];
			c++;
		}
		m++;
	}

	return new_matrix;
}

void initialize_zero(int **a, int size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			a[i][j] = 0;
	}
}

// suma a doua matrici
int **add(int **a, int **b, int size)
{
	int **result = NULL;
	result = alloc_matrix(result, size, size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int cp_a = a[i][j], cp_b = b[i][j];
			cp_a %= N_MODULO;
			cp_b %= N_MODULO;
			if (cp_a < 0)
				cp_a += N_MODULO;
			if (cp_b < 0)
				cp_b += N_MODULO;
			result[i][j] = cp_a + cp_b;
			result[i][j] %= N_MODULO;
			if (result[i][j] < 0)
				result[i][j] += N_MODULO;
		}
	}
	return result;
}

// diferenta a doua matrici
int **substract(int **a, int **b, int size)
{
	int **result = NULL;
	result = alloc_matrix(result, size, size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int cp_a = a[i][j], cp_b = b[i][j];
			cp_a %= N_MODULO;
			cp_b %= N_MODULO;
			if (cp_a < 0)
				cp_a += N_MODULO;
			if (cp_b < 0)
				cp_b += N_MODULO;
			result[i][j] = cp_a - cp_b;
			result[i][j] %= N_MODULO;
			if (result[i][j] < 0)
				result[i][j] += N_MODULO;
		}
	}
	return result;
}

// adaug rezultatele in matricea produs
void add_result(int **result, int **a, int size, int m, int n)
{
	int starting_row = m;
	int starting_coll = n;
	for (int i = 0; i < size; i++) {
		starting_coll = n;
		for (int j = 0; j < size; j++) {
			result[starting_row][starting_coll] = a[i][j];
			starting_coll++;
		}
		starting_row++;
	}
}

// eliberez matricile auxiliare din strassen
void free_all_m(int **m1, int **m2, int **m3, int **m4, int **m5, int **m6,
				int **m7, int m)
{
	m1 = free_matrix(m1, m);
	m2 = free_matrix(m2, m);
	m3 = free_matrix(m3, m);
	m4 = free_matrix(m4, m);
	m5 = free_matrix(m5, m);
	m6 = free_matrix(m6, m);
	m7 = free_matrix(m7, m);
}

// eliberez patru matrici
void free_four(int **a, int **b, int **c, int **d, int m)
{
	a = free_matrix(a, m);
	b = free_matrix(b, m);
	c = free_matrix(c, m);
	d = free_matrix(d, m);
}

// algoritmul lui strassen de inmultire
int **strassen(int **a, int **b, int m)
{
	int **c = NULL;
	c = alloc_matrix(c, m, m);
	initialize_zero(c, m);
	if (m > 1) {
		int **a11, **a12, **a21, **a22, **b11, **b12, **b21, **b22;
		// impart matricile in 4 zone egele ca dimensiune
		a11 = split_matrix(a, m, 0, 0);
		a12 = split_matrix(a, m, 0, m / 2);
		a21 = split_matrix(a, m, m / 2, 0);
		a22 = split_matrix(a, m, m / 2, m / 2);

		b11 = split_matrix(b, m, 0, 0);
		b12 = split_matrix(b, m, 0, m / 2);
		b21 = split_matrix(b, m, m / 2, 0);
		b22 = split_matrix(b, m, m / 2, m / 2);

		// apelez strassen recursiv

		int **m1, **m2, **m3, **m4, **m5, **m6, **m7;
		int **m1_a = add(a11, a22, m / 2), **m1_s = add(b11, b22, m / 2);
		int **m2_a = add(a21, a22, m / 2);
		int **m3_s = substract(b12, b22, m / 2);
		int **m4_s = substract(b21, b11, m / 2);
		int **m5_a = add(a11, a12, m / 2);
		int **m6_s = substract(a21, a11, m / 2), **m6_a = add(b11, b12, m / 2);
		int **m7_s = substract(a12, a22, m / 2), **m7_a = add(b21, b22, m / 2);
		// calculez matricile auxiliare
		m1 = strassen(m1_a, m1_s, m / 2);
		m2 = strassen(m2_a, b11, m / 2);
		m3 = strassen(a11, m3_s, m / 2);
		m4 = strassen(a22, m4_s, m / 2);
		m5 = strassen(m5_a, b22, m / 2);
		m6 = strassen(m6_s, m6_a, m / 2);
		m7 = strassen(m7_s, m7_a, m / 2);

		// calculez bucatile din matricea rezultat
		int **c11, **c12, **c21, **c22;
		int **c11_first = add(m1, m4, m / 2);
		int **c11_second = substract(c11_first, m5, m / 2);
		c11 = add(c11_second, m7, m / 2);

		c12 = add(m3, m5, m / 2);
		c21 = add(m2, m4, m / 2);

		int **c22_first = substract(m1, m2, m / 2);
		int **c22_second = add(c22_first, m3, m / 2);
		c22 = add(c22_second, m6, m / 2);

		// pun rezultatele in c

		add_result(c, c11, m / 2, 0, 0);
		add_result(c, c12, m / 2, 0, m / 2);
		add_result(c, c21, m / 2, m / 2, 0);
		add_result(c, c22, m / 2, m / 2, m / 2);

		// dau free la toate matricile
		free_all_m(m1, m2, m3, m4, m5, m6, m7, m / 2);
		free_four(c11, c12, c21, c22, m / 2);
		free_four(a11, a12, a21, a22, m / 2);
		free_four(b11, b12, b21, b22, m / 2);
		free_four(m1_a, m1_s, m2_a, m3_s, m / 2);
		free_four(m4_s, m5_a, m6_a, m6_s, m / 2);
		free_four(m7_a, m7_s, c11_first, c11_second, m / 2);
		c22_first = free_matrix(c22_first, m / 2);
		c22_second = free_matrix(c22_second, m / 2);
	} else {
		c[0][0] += a[0][0] * b[0][0];
	}
	return c;
}

/* verific alocarile si pregatesc variabilele necesare pentru
 a face inmultirea prin algoritmul strassen */
void stras_mat(int f, int s, int ***p, int *rows, int *coll, int *sum, int *n)
{
	check_allocs(rows, coll, sum, p, *n);
	int **a = p[f];
	int **b = p[s];
	rows[*n] = rows[f];
	coll[*n] = coll[s];
	int m = rows[*n];
	int **c = strassen(a, b, m);
	p[*n] = c;
	(*n)++;
}

int main(void)
{
	char input;
	int ***p_matrix = NULL, *rows = NULL, *colls = NULL, *sum = NULL;
	int num = 0, n, m, index, first, second, ok;

	while (1) {
		scanf(" %c", &input);
		switch (input) {
		case 'L':
			scanf("%d%d", &m, &n); // aloc spatiu pentru inca o matrice
			resize_all(&p_matrix, &colls, &rows, &sum, num);
			check_allocs(rows, colls, sum, p_matrix, num);
			p_matrix[num] = read_matrix(p_matrix[num], m, n);
			check_matrix(p_matrix[num], rows, colls, sum, p_matrix, num);
			update_size(rows, colls, m, n, &num);
			break;
		case 'D':
			print_matrix_size(rows, colls, num);
			break;
		case 'P':
			print_matrix(p_matrix, rows, colls, num);
			break;
		case 'C':
			check_resize(p_matrix, rows, colls, &index, num, &ok);
			if (ok == 1)
				check_matrix(p_matrix[index], rows, colls, sum, p_matrix, num);
			break;
		case 'M':
			if (check_prod(&first, &second, colls, rows, num)) {
				resize_all(&p_matrix, &colls, &rows, &sum, num);
				prod_matrix(first, second, p_matrix, rows, colls, sum, &num);
			}
			break;
		case 'O':
			sum_matrix(p_matrix, sum, num, rows, colls);
			sort_matrix(p_matrix, sum, rows, colls, num);
			break;
		case 'T':
			check_transp(p_matrix, rows, colls, num, sum);
			break;
		case 'R':
			check_pow_matrix(p_matrix, rows, colls, num, sum);
			break;
		case 'F':
			check_free(p_matrix, rows, colls, sum, num, &ok);
			if (ok == 1) {
				num = 0;
			} else if (ok == 2) { // realoc totul cu un spatiu mai putin
				num--;
				resize_all(&p_matrix, &colls, &rows, &sum, num);
				check_allocs(rows, colls, sum, p_matrix, num);
			}
			break;
		case 'Q':
			free_all(p_matrix, rows, colls, sum, num);
			exit(0);
			break;
		case 'S':
			if (check_prod(&first, &second, colls, rows, num)) {
				resize_all(&p_matrix, &colls, &rows, &sum, num);
				stras_mat(first, second, p_matrix, rows, colls, sum, &num);
			}
			break;
		default:
			printf("Unrecognized command\n");
		}
	}

	return 0;
}
