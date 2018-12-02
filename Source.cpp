#include <iostream>
#include <random>
#include <vector>
#define LIMIT 50
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

vector<int> gen_shadows(int k, int& M, int p, int n, vector <int> shadows){
	random_device random_device2;
	mt19937 generator(random_device2());
	uniform_int_distribution<> distribution(1, LIMIT);
	vector <int> arrX;
	arrX.push_back(0);
	arrX.push_back(8);
	arrX.push_back(7);


	/*
	for (int i = 0; i < k - 1; i++){
	arrX.push_back(distribution(generator));
	}
	*/

	for(int x = 1; x < (n + 1); x++){
		shadows.push_back(0);
		for(int i = k - 1; i > 0; i--){
			shadows[x] += arrX[i] * pow(x,i);
		}
		shadows[x] += M;
		shadows[x] = shadows[x] % p;
		cout << "Shadows of "<< x << " is: " << shadows[x] << endl;
	}
	// Clearing values
	M = -1;		// Delete secret
	arrX.clear();	// Delete koefficients 

	return shadows;
}

vector <int> multiply_polynoms(vector<int> polynom1,vector<int> polynom2){
	vector<int> multipliedPolynom;
	//vector<int> polynom1 = basisPolynoms[0];
	//vector<int> polynom2 = basisPolynoms[1];

	int m = 0;
	for (int i = 0; i < polynom1.size(); i++){
		for (int j = 0; j < polynom2.size(); j++){
			if (multipliedPolynom.size() <= (i + j))
				multipliedPolynom.push_back(0);
			multipliedPolynom[i + j] += (polynom1[i] * polynom2[j]);
		}
	}

	for (int i = 0; i < multipliedPolynom.size();i++){
		if (multipliedPolynom[i] < 0)
			multipliedPolynom[i] = 13 + multipliedPolynom[i];
		else
			multipliedPolynom[i] = multipliedPolynom[i] % 13;
		}

	return multipliedPolynom;
}


vector <int> calculating_basis_polynom(vector<idShadowStruct> idShadowVector, int k){

	vector<int> basisPolynom; // vector describing polynom
	vector<vector<int>> basisPolynoms;
	vector<int> multipliedPolynom;
	//basisPolynoms.push_back(basisPolynom);

	//for (int i = 1; i < k; i++){}
	int denominator = 1;
	for(int i = 0; i < k; i++){
		for (int j = 0; j < k; j++){

			if (i != j){
				basisPolynom.push_back(-idShadowVector[j].id);
				basisPolynom.push_back(1);

				basisPolynoms.push_back(basisPolynom);

				denominator *= idShadowVector[i].id - idShadowVector[j].id;
				
				basisPolynom.clear();
			}
		}
		
		multipliedPolynom = multiply_polynoms(basisPolynoms[0],basisPolynoms[1]); // (x^2 - 8x + 15)
		denominator = 9; // 3
		vector<int> denominatorVector;
		denominatorVector.push_back(denominator);
		// алгоритм евклида к знаменателю
		// 
		multipliedPolynom = multiply_polynoms(denominatorVector,multipliedPolynom); 

	}

	


	vector<int> polinomFirst;
	vector<int> polynomSecond;
	// (x - xj)/(xi - xj)

	for (int j = 0; ;j++){
		for (int i = 0; ;i++){

		}
	}


	return basisPolynom;
};


vector <int> interpolation_lagrange_polynom(vector<idShadowStruct> idShadowVector, int k){
	vector <int> lagrange_polynom;
	lagrange_polynom.push_back(-1);

	for (int i = 1; i < k; i++){
		calculating_basis_polynom(idShadowVector,k);
		lagrange_polynom[i] = idShadowVector[i].shadow;
	}

	return lagrange_polynom; 
}



void recovery_secret(){

}




int main(){
	int M;
	cout << "Input secret: ";
	//cin >> M;
	M = 11;

	//int p = gen_p(M);
	int p = 13;
	cout << p << endl;

	int n;
	cout << "Input amount of people in coalition: ";
	//cin >> n;
	n = 5;

	int k;
	cout << "Input threshold number of people in coalition: ";
	//cin >> k;
	k = 3;

	vector <int> shadows;
	shadows.push_back(-1);

	//shadows = gen_shadows(k, M, p, n, shadows);
	//shadows.clear();
	
	//recovery_secret();

	int id;
	int sh;
	char ans;
	idShadowStruct idShadow;
	vector<idShadowStruct> idShadowVector;

	for (int i = 0; i < 3; i++){
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
		/*
		reenter:
		cout << "Do you know another id and shadow of key? (y/n)" << endl;
		cin >> ans;
		if (ans != 'y' && ans != 'n'){
			cout << "Incorrect input! Try again" << endl;
			ans = NULL;
			goto reenter;
		}
			
		if (ans == n)
			break;
			*/
	}
	
	
	interpolation_lagrange_polynom(idShadowVector, k);

	system("pause");
	return 0;
}
