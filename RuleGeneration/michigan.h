
void michigan(vector <vector <string>> lista,
		flowSize sepLenRul,
		flowSize sepWidRul,
		flowSize petLenRul,
		flowSize petWidRul)
{

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Crear reglas aleatorias						|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	vector <vector <int>> rulez;
	
	random_device rd;
	
	mt19937 gen(rd());
	
	uniform_int_distribution<> disSym(0, Sym);
	uniform_int_distribution<> disRul(0, NR-1);
	uniform_int_distribution<> disCro(1, Cros);
	uniform_int_distribution<> disMut(1, Mut);
	
	
	for (int i = 0; i < NR; ++i){
		
		rulez.emplace_back(rulgen(lista, sepLenRul, sepWidRul, petLenRul, petWidRul));
	}
	
	save_csv("results/MichRules.csv", rulez);

int count = 1;

while (count <= Ite)
{
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Genereación de reglas y razonamiento difuso			|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	vector <vector <double>> uAqxp_CFq;
	vector <vector <double>> uAwxpCFw;
	
	/*----Variables names-----
	uAqxp_CFq[0]->PrClass1
	uAqxp_CFq[1]->PrClass2
	uAqxp_CFq[2]->PrClass3
	uAqxp_CFq[3]->PrClassCq
	uAqxp_CFq[4]->PrClassNCq
	uAqxp_CFq[5]->CFq
	*/
	
	for (int i = 0; i < NR; ++i)
	{
		vector <double> uAqxp_row (Ant,0);
		vector <double> CFq_row (6,0);

		for (int j = 0; j < lista.size(); ++j)
		{
			double uAqxp;
			
			double sepLen = stof(lista[j][0]);
			double sepWid = stof(lista[j][1]);
			double petLen = stof(lista[j][2]);
			double petWid = stof(lista[j][3]);

			/*cout << sepLen << " " << sepWid <<  " " << petLen <<  " " << petWid << endl;
			
			cout << rulez[i][0] << " " << rulez[i][1] << " " << rulez[i][2] << " " << rulez[i][3] << " " << endl;
			
			cout << sepLenRul.setsVec[rulez1 - 2] << " " << sepLenRul.setsVec[rulez1 - 1] << endl;
			cout << sepWidRul.setsVec[rulez2 - 2] << " " << sepWidRul.setsVec[rulez2 - 1] << endl;
			cout << petLenRul.setsVec[rulez3 - 2] << " " << petLenRul.setsVec[rulez3 - 1] << endl;
			cout << petWidRul.setsVec[rulez4 - 2] << " " << petWidRul.setsVec[rulez4 - 1] << endl << endl;
			
			cout << sepLenRul.setsY[rulez1 - 2] << " " << sepLenRul.setsY[rulez1 - 1] << endl;
			cout << sepWidRul.setsY[rulez2 - 2] << " " << sepWidRul.setsY[rulez2 - 1] << endl;
			cout << petLenRul.setsY[rulez3 - 2] << " " << petLenRul.setsY[rulez3 - 1] << endl;
			cout << petWidRul.setsY[rulez4 - 2] << " " << petWidRul.setsY[rulez4 - 1] << endl << endl;*/
			
			uAqxp = compGrade(rulez[i], sepLen, sepWid, petLen, petWid,
						&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, 0);
			
			//cout << uAqxp << " " << uAq1xp1 << " " << uAq2xp2 << " " << uAq3xp3 << " " << uAq4xp4 << endl;
			
			uAqxp_row[0] += uAqxp;
			
			if (lista[j][4] == "Iris-setosa")
				uAqxp_row[1] += uAqxp;
			else if (lista[j][4] == "Iris-versicolor")
				uAqxp_row[2] += uAqxp;
			else if (lista[j][4] == "Iris-virginica")
				uAqxp_row[3] += uAqxp;
		}
		
		//Fuzzy conditional probability for each class
		CFq_row[0] = uAqxp_row[1] / uAqxp_row[0]; //PrClass1 setosa
		CFq_row[1] = uAqxp_row[2] / uAqxp_row[0]; //PrClass2 versicolor
		CFq_row[2] = uAqxp_row[3] / uAqxp_row[0]; //PrClass3 virginica
		
		//Consequent Class Cq of the fuzzy rule Rq
		CFq_row[3] = max(CFq_row[0], max(CFq_row[1], CFq_row[2])); //PrClassCq
		
		//Complement of the consequent Class Cq
		CFq_row[4] = CFq_row[0] + CFq_row[1] + CFq_row[2] - CFq_row[3]; // PrClassNCq
		
		//Certainty Factor (Rule weight)		
		/*if (rulez[i][4] == 0)
			CFq_row[5] = 0; //CFq
		else*/
		CFq_row[5] = CFq_row[3] - CFq_row[4]; //CFq
		
		uAqxp_CFq.emplace_back(CFq_row);	//PrClass1, PrClass2, PrClass3, PrClassCq, PrClassNCq, CFq, uAwxpCFw

	}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Evaluación de reglas						|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	for (int i = 0; i < lista.size(); ++i)
	{
		double sepLen = stof(lista[i][0]);
		double sepWid = stof(lista[i][1]);
		double petLen = stof(lista[i][2]);
		double petWid = stof(lista[i][3]);
		
		vector <double> uAwxpCFw_row (6,0);
		
		for (int j = 0; j < NR; ++j)
		{
			double uAqxp = compGrade(rulez[j], sepLen, sepWid, petLen, petWid,
					&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, 0);
			
			if (uAwxpCFw_row[0] < uAqxp_CFq[j][5] * uAqxp){ //uAqxp_CFq[5]->CFq
				uAwxpCFw_row[0] = uAqxp_CFq[j][5] * uAqxp;
				uAwxpCFw_row[1] = rulez[j][0];
				uAwxpCFw_row[2] = rulez[j][1];
				uAwxpCFw_row[3] = rulez[j][2];
				uAwxpCFw_row[4] = rulez[j][3];
				uAwxpCFw_row[5] = rulez[j][4]; // Clase (setosa, versicolor, virginica)
			}
		}
		
		uAwxpCFw.emplace_back(uAwxpCFw_row);
	}


	//save_csv("results/Mprobability.csv", uAqxp_CFq);
	
	//save_csv("results/Mclassified.csv", uAwxpCFw);
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Aptitud (fitness) de reglas					|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	vector <int> fitness (NR,0);
	
	int aciertos = 0;
	
	for (int i = 0; i < lista.size(); ++i)
	{
		double clase;
		
		if (lista[i][4] == "Iris-setosa")
			clase = 1;
		else if (lista[i][4] == "Iris-versicolor")
			clase = 2;
		else if (lista[i][4] == "Iris-virginica")
			clase = 3;
		
		if (uAwxpCFw[i][5] == clase){
			++aciertos;
			for (int j = 0; j < NR; ++j) {
				if (uAwxpCFw[i][1] == rulez[j][0] && uAwxpCFw[i][2] == rulez[j][1] && 
					uAwxpCFw[i][3] == rulez[j][2] && uAwxpCFw[i][4] == rulez[j][3] && 
					uAwxpCFw[i][5] == rulez[j][4])
					fitness[j] += 1;
			}
		}
	}
	
	//Imprimir cantidad de aciertos por regla
	cout << "Aciertos: " << aciertos << endl;
	
	for (int i = 0; i < fitness.size(); ++i)
	{
		cout << fitness[i] << " ";
	}
	cout << endl;
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Mating pool y selección por torneo				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	vector <double> parent1;
	vector <double> parent2;
	
	// variables temporales para guardar los índices de los elementos de las matrices
	int t1, t2;
	
	/*//k = 3 the tournament size
	int t1 = rand()%15;
	int t2 = rand()%15;
	int t3 = rand()%15;
	if (fitness[t1] > fitness[t2]){
		if (fitness[t1] > fitness[t3])
			for (int i = 0; i < 5; ++i)
				parent1.emplace_back(rulez[t1][i]);
		else
			for (int i = 0; i < 5; ++i)
				parent1.emplace_back(rulez[t3][i]);
	}
	else{
		if (fitness[t2] > fitness[t3])
			for (int i = 0; i < 5; ++i)
				parent1.emplace_back(rulez[t2][i]);
		else
			for (int i = 0; i < 5; ++i)
				parent1.emplace_back(rulez[t3][i]);
	}
	
	parent2 = parent1;
	while (parent1 == parent2){
		parent2.clear();
		t1 = rand()%15;
		t2 = rand()%15;
		t3 = rand()%15;
		if (fitness[t1] > fitness[t2]){
			if (fitness[t1] > fitness[t3])
				for (int i = 0; i < 5; ++i)
					parent2.emplace_back(rulez[t1][i]);
			else
				for (int i = 0; i < 5; ++i)
					parent2.emplace_back(rulez[t3][i]);
		}
		else{
			if (fitness[t2] > fitness[t3])
				for (int i = 0; i < 5; ++i)
					parent2.emplace_back(rulez[t2][i]);
			else
				for (int i = 0; i < 5; ++i)
					parent2.emplace_back(rulez[t3][i]);
		}
	}*/
	
	// Creación de mating pool
	vector <vector<double>> matiPool;
	vector<double> mask = {0, 1, 0, 1, 1};
	
	for(int i = 0; i < NR; ++i){
		t1 = disRul(gen);
		t2 = disRul(gen);
		
		vector<double> row;

		for (int j = 0; j < Ant+1; ++j){
			// Selección por torneo binario
			if (fitness[t1] > fitness[t2])	
				row.emplace_back(rulez[t1][j]);
			else
				row.emplace_back(rulez[t2][j]);
		}
		matiPool.emplace_back(row);
	}
	
	//save_csv("results/MmatingPool.csv", matiPool);
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Crossover							|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	t1 = disRul(gen);
	t2 = disRul(gen);
	
	/*//Show parents and mask
	cout << t1 << " " << t2 << endl;
	
	cout << "Mating 1: ";
	for (int i = 0; i < matiPool[0].size(); ++i)
	{
		cout << matiPool[t1][i] << " ";
	}
	cout << endl;
	
	cout << "Mating 2: ";
	for (int i = 0; i < matiPool[0].size(); ++i)
	{
		cout << matiPool[t2][i] << " ";
	}
	cout << endl;
	
	cout << "Mask: ";
	for (int i = 0; i < mask.size(); ++i)
	{
		cout << mask[i] << " ";
	}
	cout << endl;*/
	
	// Se eligen 2 parejas de la mating pool y se cruzan se la probabilidad de crossover
	int crossProb = disCro(gen);
	
	if (crossProb < Cros){ //Crossover probability: 0.9
		for (int i = 0; i < Ant+1; ++i){
			if (mask[i] == 1){
				parent1.emplace_back(matiPool[t1][i]);
				parent2.emplace_back(matiPool[t2][i]);
			}
			else{
				parent1.emplace_back(matiPool[t2][i]);
				parent2.emplace_back(matiPool[t1][i]);
			}
		}
	}
	else{
		for (int i = 0; i < Ant+1; ++i){
			parent1.emplace_back(matiPool[t1][i]);
			parent2.emplace_back(matiPool[t2][i]);
		}
	}
	
	/*//Show crossed parents
	cout << "Parent 1: ";
	for (int i = 0; i < parent1.size(); ++i)
	{
		cout << parent1[i] << " ";
	}
	cout << endl;
	
	cout << "Parent 2: ";
	for (int i = 0; i < parent2.size(); ++i)
	{
		cout << parent2[i] << " ";
	}	
	cout << endl;*/
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Mutation							|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	for (int i = 0; i < Ant; ++i){//Mutate the n antecedents (n-dimensional patterns) but not the class
		int mutProb = disMut(gen);
		if(mutProb < 2){//Mutation Probability: 1/n = 1/4
			parent1[i] = disSym(gen);
		}
		mutProb = rand()%Mut;
		if(mutProb < 2){
			parent2[i] = disSym(gen);
		}
	}
	
	/*//Show mutated parents
	cout << "Parent 1: ";
	for (int i = 0; i < parent1.size(); ++i)
	{
		cout << parent1[i] << " ";
	}
	cout << endl;
	
	cout << "Parent 2: ";
	for (int i = 0; i < parent2.size(); ++i)
	{
		cout << parent2[i] << " ";
	}
	cout << endl;*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Replace offsprings in mating pool				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	/*//Test with random numbers
	for(int i = 0; i < 15; ++i){
		fitness[i] = rand()%15;
		cout << fitness[i] << endl;
	}
	cout << t1 << endl;
	cout << t2 << endl;*/

	//Encontrar las reglas con el menor fitness
	t1 = 0;
	for(int i = 0; i < NR; ++i){
		if (fitness[i] < fitness[t1])
			t1 = i;
	}
	
	t2 = abs(t1 - 1);
	for(int i = 0; i < NR; ++i){
		if (i != t1 && fitness[i] < fitness[t2])
			t2 = i;
	}
	
	//Sólo reemplazar reglas si los descendientes no están repetidos en la población
	int ct1;
	int ct2;
	for (int i = 0; i < NR; ++i){
		ct1 = 0;
		for (int j = 0; j < Ant+1; ++j){
			if (rulez[i][j] == parent1[j])
				ct1++;
		}
		if (ct1 == 5)
			break;
	}
	
	for (int i = 0; i < NR; ++i){
		ct2 = 0;
		for (int j = 0; j < Ant+1; ++j){
			if (rulez[i][j] == parent2[j]);
				ct2++;
		}
		if (ct2 == 5)
			break;
	}
	
	if (ct1 != 5){
		for (int i = 0; i < Ant+1; ++i){
			rulez[t1][i] = parent1[i];
		}
	}
	
	if (ct2 != 5){
		for (int i = 0; i < Ant+1; ++i){
			rulez[t2][i] = parent2[i];
		}
	}
	
/*	for (int i = 0; i < Ant+1; ++i){
		rulez[t1][i] = parent1[i];
		rulez[t2][i] = parent2[i];
	}*/
	
	++count;
	
}

	save_csv("results/MichRulesB.csv", rulez);

	return;
}
