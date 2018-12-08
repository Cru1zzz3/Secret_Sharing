#include <iostream>
#include <random>
#include <vector>
#define LIMIT 100
using namespace std;

struct idShadowStruct {
	int id;
	int shadow;
};

bool prime(int n){ 
	for(int i = 2;i <= sqrt(n); i++)
		if(n % i == 0)
			return false;
	return true;
}

unsigned int gen_p(int M){
	int rndNum; 
	bool primeCheck = false;
	random_device random_device;
	mt19937 generator(random_device());
	uniform_int_distribution<> distribution(M, LIMIT);

	while (!primeCheck){
		rndNum = distribution(generator);
		primeCheck = prime(rndNum);
	}
	return rndNum;
};

int gcd (int a, int b, int & x, int & y) {
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}
	int x1, y1;
	int d = gcd (b%a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

int reverse_element(int a, int N){
	int x, y;
	int g = gcd (a, N, x, y);
	if (g != 1)
		cout << "Reverse element in denominator!! no solution!!!" << endl;
	else {
		x = (x % N + N) % N;
		return x;
	}
}

vector<int> gen_shadows(int k, int& M, int p, int n, vector <int> shadows){
	random_device random_device2;
	mt19937 generator(random_device2());
	uniform_int_distribution<> distribution(1, LIMIT);
	vector <int> arrX;
	arrX.push_back(0);

	for (int i = 0; i < k - 1; i++)
		arrX.push_back(distribution(generator));

	for(int x = 1; x < (n + 1); x++){
		shadows.push_back(0);
		for(int i = k - 1; i > 0; i--){
			shadows[x] += arrX[i-1] * pow(x,i-1);
		}
		shadows[x] += M;
		shadows[x] = shadows[x] % p;
		cout << "Shadows of "<< x << " is: " << shadows[x] << endl;
	}
	// Clearing values
	M = -1;		
	arrX.clear();	

	return shadows;
}

vector <int> add_polynoms(vector<int> polynom1,vector<int> polynom2,int N){
	vector<int> sumPolynom;
	for (int i = 0; i < polynom1.size(); i++){
		int sum = (polynom1[i] + polynom2[i]) % N;
		sumPolynom.push_back(sum);
	}
	return sumPolynom;
}

vector <int> multiply_polynoms(vector<int> polynom1,vector<int> polynom2,int N){
	vector<int> multipliedPolynom;
	int m = 0;
	for (int i = 0; i < polynom1.size(); i++){
		for (int j = 0; j < polynom2.size(); j++){
			if (multipliedPolynom.size() <= (i + j))
				multipliedPolynom.push_back(0);
			multipliedPolynom[i + j] += (polynom1[i] * polynom2[j]);
		}
	}
	return multipliedPolynom;
}

vector <int> calculating_lagrange_polynom(vector<idShadowStruct> idShadowVector, int k,vector<int> lagrangePolynom, int N){

	for(int i = 0; i < k; i++){
		vector<int> basisPolynom; // vector describing polynom
		vector<vector<int>> basisPolynoms;
		vector<int> multipliedPolynom;
		int denominator = 1;		
		for (int j = 0; j < k; j++){

			if (i != j){
				basisPolynom.push_back(-idShadowVector[j].id);
				basisPolynom.push_back(1);

				basisPolynoms.push_back(basisPolynom);

				denominator *= idShadowVector[i].id - idShadowVector[j].id;

				basisPolynom.clear();
			}
		}

		for (int j = 0; j < k - 2; j++){ //!!!
			basisPolynoms[j+1] = multiply_polynoms(basisPolynoms[j],basisPolynoms[j + 1],N); // (x^2 - 8x + 15)
			multipliedPolynom = basisPolynoms[j + 1];
		}

		for (int i = 0; i < multipliedPolynom.size();i++){
			if (multipliedPolynom[i] < 0)
				multipliedPolynom[i] = N + multipliedPolynom[i];
			else
				multipliedPolynom[i] = multipliedPolynom[i] % N;
		}

		// reverse modulus denominator

		if (denominator < 0)
			while (denominator < 0)
			{
				denominator = N + denominator;
			}
			
		else
			denominator = denominator % N;

		denominator = reverse_element(denominator,N); // 3
		vector<int> denominatorVector;
		denominatorVector.push_back(denominator);

		multipliedPolynom = multiply_polynoms(denominatorVector,multipliedPolynom,N);

		//summing lagrange summand polynoms
		vector<int> convertToVector;
		convertToVector.push_back(idShadowVector[i].shadow);
		vector<int> lagrangeSummand = multiply_polynoms(convertToVector,multipliedPolynom,N);

		while (lagrangePolynom.size() < lagrangeSummand.size())
			lagrangePolynom.push_back(0);
		lagrangePolynom = add_polynoms(lagrangeSummand,lagrangePolynom,N);
	}
	return lagrangePolynom;
};

int recovery_secret(vector<idShadowStruct> idShadowVector, int k,int N){
	vector<int> lagrangePolynom;
	lagrangePolynom = calculating_lagrange_polynom(idShadowVector, k,lagrangePolynom,N);

	int secret = lagrangePolynom[0];
	return secret;
}

int main(){

	int M;
	cout << "Input secret: ";
	cin >> M;
	
	int p = gen_p(M);
	cout << "Module: " << p << endl;

	int n;
	cout << "Input amount of people in coalition: ";
	cin >> n;
	
	int k;
	cout << "Input threshold number of people in coalition: ";
	cin >> k;
	
	vector <int> shadows;
	shadows.push_back(-1);

	shadows = gen_shadows(k, M, p, n, shadows);
	shadows.clear();

	int id;
	int sh;
	char ans;
	idShadowStruct idShadow;
	vector<idShadowStruct> idShadowVector;

	int N;
	cout << endl << endl;
	cout << "Input module: ";
	cin	 >> N;
	
	cout << "Input k: ";
	cin >> k;
	cout << endl;
	for (int i = 0; i < k; i++){
		cout << "Input id: ";
		cin	 >> idShadow.id;
		if (idShadow.id == 'n')
			break;
		cout << "Input shadow of key: ";
		cin	 >> idShadow.shadow;
		if (idShadow.shadow == 'n')
			break;
		cout << endl;
		idShadowVector.push_back(idShadow);
	}

	int secret = recovery_secret(idShadowVector,k , N);
	cout << "Secret is: " << secret << endl;
	system("pause");
	return 0;
}
