/* redeflix.cpp --- Trabalho prático GCC224.
 * Autor: Pedro Eduardo Garcia (Eu até tinha uma dupla a princípio, mas acabei tendo que fazer o trabalho sozinho)
 * 
 * O redeflix.cpp é um programa que simula uma lista de filmes criada e mantida pelo usuário utilizanto arquivos persistentes
 * Utilizamos arquivos binários para simular um banco de dados onde ficam guardadas as informações pertinentes aos filmes que se encontram na lista
 * Cada filme (struct) é caracterizado por nome, ano de lançamento, tipo, duração, diretor do filme, descrição do filme, avaliação e se o mesmo já foi visto pelo usuário
 * 
 * Na primeira utilização do programa é possível criar um arquivo vazio para começar a adicionar os filmes
 * ou deixar o redeflix criar o própiro arquivo com alguns filmes já contidos nele
 * 
 * Verificado o arquivo que podemos trabalhar o usuário estará apto a começar a explorar as funcionalidades do programa.
 * 
 * Escrevemos o programa para funcionar dentro de um loop que aciona um switch e case de menus e sub menus e só fecha o loop quando selecionado a saida e encerramento do mesmo.
 * 
 * Podemos listar todos os filmes do nosso arquivo, podemos listar os filmes assistidos e podemos listar os filmes não assistidos
 * Podemos buscar por um filme para lista-lo na tela ou alterar o seu conteudo, seja ele qual for.
 * Podemos adicionar um filme novo ao nosso arquivo
 * Podemos ordenar o nosso arquivo 
 * Podemos, por fim, excluir um filme do nosso arquivo
 * 
 * O controle e ordenação dos filmes é baseado no nome, por exemplo, não podemos excluir um filme cujo nome nao se encontra
 * no nosso arquivo, assim como não podemos adicionar um filme cujo nome já se encontra no arquivo. A ordenação, da mesma forma, é feita com base na
 * ordem alfabética de forma crescente.
 * 
 * Para a requisição e controle das entradas dos usuários temos as funções nomeadas por get*ALGO*() e são as primeiras funções a serem definidas com dois templates
 * basicos, um para controle de strings, onde se respeita os limites estipulados (por exemplo, nome é char nome[80], logo um nome não pode ter um tamanho maior que 80
 * e menor que 0) e outro para o controle de outros tipos de dados, seja int ou float. Dito isso, não existe controle para o que o usuario quer colocar em cada campo, 
 * se o usuário decidir que o ano de lançamento de determinado filme é 1123441, o lançamento do filme ficará marcado no arquivo com essa entrada, se decidir que o ano
 * é 1.12, será requisitado a entrada novamente.
 * 
 * De modo geral, só se abre o arquivo uma vez que é fechado quando se decide sair e encerrar o programa, exceto na exclusão de um filme do arquivo.
 * Na exclusão de um filme, apesar de ter recebido resposta do prof. Schneider no forum eu resolvi seguir com a solução que eu encontrei a principio, utilizamos um 
 * arquivo temporário para salvar nosso banco de dados com a exclusão seguidos por rename e remove. Entendo que não é exatamente o que foi pedido pelos professores,
 * mas eu ja tinha boa parte do programa feito e até onde eu analisei eu teria que reescrever grande parte dos procedimentos caso eu adicionasse um campo na struct para o controle
 * de qual filme, ou quantos filmes estariam excluidos, de forma a, na exclusão, colocá-los, ou no final do arquivo, ou apenas marcar o campo apropriado para que fosse ignorado das buscas. 
 */ 

#include <iostream> 	// cin cout
#include <fstream> 		// iof streams arquivo
#include <cstring> 		// strcpy();
#include <string> 		// string para check: solução para a comparação de chaves de busca pelo nome, 
						// não consegui comparar o nome do filme tirado do arquivo com por exemplo um char[80] que seria equivalente ao filme.nome 
#include <stdio.h> 		// remove() e rename()
#include <limits> 		// numeric_limits: solução utilizada para garantia de entrada de dado do tipo correto

using namespace std;

// Dados para a criação do arquivo inicial se for a escolha feita pelo usuário
// se trata de vetores de dados que criam as structs filmes, como por exemplo o nome, o ano e a duração do filme em questão
// os dados foram retirados do IMDb, os nomes estão em inglês, assim como os tipos
// as descrições não precisam ser levadas a sério, as originais são ligeiramente grandes e estariam em inglês, copiar e colar em português seria necessário redigitar com todos os acentos removidos, então optei por algo mais descontraído

extern const char nomes[20][80] = {		//nomes dos filmes do arquivo inicial
	"The Shawshank Redemption","The Dark Knight","Inception","Pulp Fiction","The Lord of the Rings: The Fellowship of the Ring",
	"The Godfather","Interstellar","The Silence of the Lambs","The Wolf of Wall Street","Shutter Island","Titanic","Joker",
	"Deadpool","Avengers: Infinity War","Avengers: Endgame","Avengers: Age of Ultron","Harry Potter and the Sorcerer's Stone",
	"Thor: Ragnarok","Parasite","Once Upon a Time... In Hollywood"
};
								 
extern const int anos[20] = {		//anos de lançamento dos filmes do arquivo inicial
	1994,2008,2010,1994,2001,1972,2014,1991,
	2013,2010,1997,2019,2016,2018,2019,2015,
	2001,2017,2019,2019
};
							 
extern const char tipos[20][80] = {		//tipos dos filmes do arquivo inicial
	"Drama","Action Crime Drama","Action Adventure Sci-Fi","Crime Drama",
	"Action Adventure Drama","Crime Drama","Adventure Drama Sci-Fi",
	"Crime Drama Thriller","Biography Crime Drama","Mystery Thriller",
	"Drama Romance","Crime Drama Thriller","Action Adventure Comedy",
	"Action Adventure Sci-Fi","Action Adventure Drama","Action Adventure Sci-Fi",
	"Adventure Family Fantasy","Action Adventure Comedy","Comedy Drama Thriller",
	"Comedy Drama"
};
				
extern const int duras[20] = {		//duração dos filmes do arquivo inicial
	142,152,148,154,178,175,169,118,180,
	138,194,122,108,159,181,141,152,130,
	132,161
};

extern const char dires[20][80] = {		//diretores dos filmes do arquivo inicial
	"Frank Darabont","Christopher Nolan","Christopher Nolan","Quentin Tarantino",
	"Peter Jackson","Francis Ford Coppola","Christopher Nolan","Jonathan Demme",
	"Martin Scorsese","Martin Scorsese","James Cameron","Todd Phillips",
	"Tim Miller","Anthony Russo","Anthony Russo","Joss Whedon",
	"Chris Columbus","Taika Waititi","Bong Joon Ho","Quentin Tarantino"
};

extern const char descris[20][200] = {		//descrição dos filmes do arquivo inicial
	"Prisao, corredor da morte, coisas estranhas acontecem. Stephen King",
	"Batman com Coringa, segundo de dois da trilogia, sim, dois, vamos deixar por isso mesmo",
	"Entra no sonho, pra entrar no sonho, de um outro sonho para roubar o segredo enquanto sai do sonho que esta no sonho do sonho.... ?",
	"LostJohnTravolta.gif",
	"Anao devolve o anel, nao, espera, spoiler! Anao talvez devolve o anel",
    "Poderoso Chefao, nem precisa de descricao, tem mais de um, mas esse que a o bom",
    "A poeira esta atacando o mundo e nosso heroi foge da terra para salva-la",
    "O silencio das ovelhas. Canibal contra FBI!",
    "Leonardo DiCaprio no dia a dia de um corretor em wall street",
    "Leonardo DiCaprio no dia a dia de um biruto em uma ilha",
    "Leonardo DiCaprio no dia a dia de uma pessoa em um barco",
    "Coringa sem batman mas com a mae",
    "Filme de heroi, hahaha, deadpool",
    "Avengers, sao tantos, mas eles lutam contra alguma coisa do espaço que quer destruir a terra ou nao sei",
    "Avengers, endgame? e ultimo! ................. ou nao",
    "Avengers..... iron hulk? Ameaca do espaco, todo mundo ja sabe",
    "Harry Potter atras de uma pedra",
    "Avengers, mas sem todos eles",
    "Familia mora escondido dentro da casa de outra familia",
    "Era uma vez em hollywood em 1969 na epoca do charles manson"
};
								   
extern const int vistos[20] = {		//marcação de já visto dos filmes do arquivo inicial
						1,1,1,1,1,
						1,1,1,1,1,
						1,1,1,1,1,
						1,0,1,1,1
};

extern const float avals[20] = {		//avaliação dos filmes do arquivo inicial
	9.3,9.0,8.8,8.9,8.8,9.2,8.6,8.6,
	8.2,8.2,7.8,8.4,8.0,8.4,8.4,7.3,
	7.6,7.9,8.6,7.6
};

// struct que representa nossos filmes, nome do filme, ano de lançamento, tipo, duração, diretor, breve descrição, avaliação e marcação de se ja foi assistido																													 
struct  filme{
	char nome[80];
	int ano;
	char tipo[80];
	int dura;
	char dire[80];
	float aval;
	char descri[200];
	int visto;
};

/* Função string getNome()
 * - return : entrada de nome de filme feita pelo usuário
 * 
 * Checamos o tamanho da entrada provida pelo usuário, limpamos o buffer de entrada do cin e pedimos para o cin
 * pegar strings com espaço em branco, aceitando o fim da entrada somente com o enter quando pressionado pelo usuário
 * 
 * Todas as funções que gerencia a entrada de dados de string são feitas da mesma forma, getTipo(), getDire() e getDescri(),
 * a única diferença é a checagem de tamanho da descrição que pode ser até 200 caracteres
 */
string getNome(){
	string key;
	while(key.length() >80 or key.length()<=0){
		cin.clear();
		while(cin.get() != '\n'){
			continue;
		}
		getline(cin, key);
		if(key.length() > 80){
			cout << "Iii... parece que voce digitou algo que e maior que o tamanho associado a *NOME* tamanho máximo *80*, tente novamente..." << endl;
			cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
			cout << "Entre com o nome do filme..." << endl;
		}
	}
	return key;
}

/* Função string getAno()
 * - return : entrada do ano do filme feita pelo usuário
 * 
 * Aqui checamos a entrada pelo tipo provido pelo usuário, só serão aceitos int. Enquanto não existe uma entrada apropriada
 * o lop while continua, dando as instruções.
 * 
 * Da mesma forma que dito na função anterior, o gerenciamento de entradas de dados que não são strings 
 * é feito dessa forma, getDura(), getAval(), getVisto(), a diferença só está presente na checagem do tipo do dado.
 */
int getAno(){
	int key;
	
	while(!(cin>>key)){
		cout << "Iii... parece que voce digitou algo que nao e um *ANO* *INT*, tente novamente..." << endl;
		cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return key;
}

string getTipo(){
	string key;
	
	while(key.length() > 80 or key.length() <= 0){
		cin.clear();
		while(cin.get() != '\n'){
			continue;
		}
		getline(cin, key);
		if(key.length() > 80){
			cout << "Iii... parece que voce digitou algo que e maior que o tamanho associado a *TIPO* tamanho maximo *80*, tente novamente..." << endl;
			cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
			cout << "Entre com o tipo do filme..." << endl;
		}
	}
	return key;
}

int getDura(){
	int key;
	
	while(!(cin>>key)){
		cout << "Iii... parece que voce digitou algo que nao e um *DURACAO* *INT*, tente novamente..." << endl;
		cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	
	return key;
}

string getDire(){
	string key;
	while(key.length() > 80 or key.length() <=0){
	cin.clear();
		while(cin.get() != '\n'){
			continue;
		}
		getline(cin, key);
		if(key.length() > 80){
			cout << "Iii... parece que voce digitou algo que e maior que o tamanho associado a *DIRETOR* tamanho maximo *80*, tente novamente..." << endl;
			cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
			cout << "Entre com o diretor do filme..." << endl;
		}
	}
	return key;
}

float getAval(){
	float key;
	while(!(cin>>key)){
		cout << "Iii... parece que voce digitou algo que nao e um *AVALIACAO* *FLOAT*, tente novamente..." << endl;
		cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return key;
}

string getDescri(){
	string key;

	while(key.length() > 200 or key.length() <=0){
		cin.clear();
		while(cin.get() != '\n'){
			continue;
		}
		getline(cin, key);
		if(key.length() > 200){
			cout << "Iii... parece que voce digitou algo que e maior que o tamanho associado a *DESCRICAO* tamanho maximo *200*, tente novamente..." << endl;
			cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
			cout << "Entre com a descricao do filme..." << endl;
		}
	}
	return key;
}

int getVisto(){
	int key;
	while(!(cin>>key)){
		cout << "Iii... parece que voce digitou algo que nao e um *VISTO* *INT*, tente novamente..." << endl;
		cout << "Se o problema persistir, de uma olhada nas instrucoes (Menu Principal ( 6 ))"<< endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return key;
}

/*	Função void db();
 *  Realizada quando não é encontrado nosso arquivo binário na pasta em que
 * 	se encontra nosso programa cpp
 *  
 *  Armazena os dados dos filmes que vão representar nosso banco de dados em um vetor de filmes
 * 	Cria nosso arquivo binário e armazena esses valores nele. redeflix.dat representa o nosso banco de dados
 * 
 * 	Essa função só é chamada quando não é encontrado o .dat, apenas na primeira vez que o programa for utilizado
 * 	ou caso o .dat seja deletado
 */
void db(){
	//número de filme
	int qtd = 20;
	//vetor buffer para escrever os filmes no arquivo
	filme buffer[qtd];
	fstream arq;
	
	//armazena no vetor de buffer os dados dos vetores de criação
	for(int i=0;i<qtd;i++){
		strcpy(buffer[i].nome, nomes[i]);
		buffer[i].ano = anos[i];
		strcpy(buffer[i].tipo, tipos[i]);
		buffer[i].dura = duras[i];
		strcpy(buffer[i].dire, dires[i]);
		strcpy(buffer[i].descri, descris[i]);
		buffer[i].aval = avals[i];
		buffer[i].visto = vistos[i];
	}
	
	for(int i=0;i<qtd;i++){
		cout << buffer[i].nome << endl;
		cout << buffer[i].ano << endl;
		cout << buffer[i].tipo << endl;
		cout << buffer[i].dura << endl;
		cout << buffer[i].dire << endl;
		cout << buffer[i].aval << endl;
		cout << buffer[i].descri << endl;
		cout << buffer[i].visto << endl;
	}
	//abre/cria arquivo (cria o arquivo no caso abaixo, já que não foi encontrado o nosso ;dat)
	arq.open("redeflix.dat", ios::out| ios::binary);
	if(!arq) cout << "Parece que ocorreu um erro abrindo arquivo..." << endl;
	
	//escreve no arquivo os dados do buffer, cada buffer tem o tamanho de um filme () e faremos isso * a quantidade de filmes
	arq.write(reinterpret_cast<char*> (buffer), sizeof(filme)* qtd);
	
	//fecha arquivo
	arq.close();
}

/*	Função int dbsize(fstream& arq); 
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- return : numero de filmes no banco de dados, tipo int
 * 
 *  Chamada uma vez no começo do loop, verifica pelo tamanho da struct filmes
 * 	quantas delas existem quando verificamos o tamanho total do arquivo
 * 	
 * Chamada após adição de novo filme ao arquivo, afim de renovar o número
 * 
 * 	Armazena em size, que apesar de ser do tipo streamsize pode ser retornado e utilizado como int no decorrer do  programa,
 * 	o valor total do tamanho do arquivo/pelo tamanho de cada filme, gerando o numero total de filmes
 */
int dbsize(fstream& arq){
	streamsize size;
	
	//aponta e indica a posição final do arquivo e divide pelo tamanho que cada filme ocupa no arquivo binário
	arq.seekg(0, arq.end);
	size = arq.tellg();
	size = size/sizeof(filme);
	
	//retorna valor calculado acima
	return size;
}

/*	Função int busca(fstream& arq, int size);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- int size = dado retornado pela chamada do dbsize no loop principal, numero de filmes no nosso arquivo
 *  - return : pos ou -1 se não achar, tipo int
 * 
 *  Chamada quando buscamos algum filme em nosso arquivo. No caso de exclusão, no caso de imprimir apenas um filme,
 * 	no caso de alteração. Em geral as buscas de um filme são realizadas com a chamada dessa função. Retorna no final a posição
 * 	em que se encontra o começo da struct do filme no arquivo binário ou -1 se não encontrar
 * 
 * 	Quando chamada, realiza a entrada de um nome dado pelo usuário, aponta o flag de leitura para o inicio do arquivo e 
 *  faz uma busca sequencial até encontrar e retorna a posição em que foi encontrado o filme ou -1 caso não encontre
 * 
 * 	Nesta função temos o primeiro controle de entrada do usuário. Associamos key ao PLACE_HOLDER para este ter um tamanho maior que o maximo aceitavel
 *  e, enquanto esta tiver um .length() maior que 80, será requisitada a entrada novamente. O segundo controle nesse caso eu acredito que nunca seria pego, 
 * 	já que qualquer entrada pode ser considerada um string, não obstante, fazemos o check dessa entrada para conferir se se trata de uma string
 */
int busca(fstream& arq, int size){
	
	int aux;
	filme buffer;
	string key,key2;
	
	cout << "Entre com o nome do filme..." << endl;
	
	key = getNome();
	
	//flag para leitura no inicio do arquivo
	arq.seekg (0);

	//multiplicamos a iteração pelo tamanho do filme para encontrar o ponto inicial de cada filme
	//salvamos o ponto de leitura no inicio da mesma com aux, lemos os dados e guardamos no nosso buffer
	//e comparamos o nome do buffer com a entrada do usuário
	for(int i=0;i<size;i++){
		arq.seekg(i*sizeof(filme),arq.beg);
		aux = arq.tellg();
		
		arq.read((char*)(&buffer), sizeof(filme));
		//key2 = buffer.nome;
		//retorna a posição se achou
		if(buffer.nome == key) return aux;	
	}
	//retorna -1 se não achou
	return -1;
}

/*	No trecho a seguir temos duas funções de imprimir em tela, a primeira delas imprime todos os filmes no arquivo e todos os
 * 	dados relativos a cada filme e na segunda, de acordo com um dos parametros mostramos todos os filmes vistos ou não vistos e todos os
 * 	dados de cada um deles
 * 
 * 	Note que temos duas formas diferentes de impressão, e sim, uma delas a gente utiliza vetor pegando todo o arquivo de uma vez só
 * 	e guardando na memória para impressão e na outra a gente utiliza apenas um buffer e itera pelo arquivo. A utilização de duas maneiras diferentes
 * 	é feita dado que me foi orientado pela professora via email que quando se le do arquivo **arq.read ((char*) (buffer), size*sizeof(filme));** dessa maneira
 * 	é mais economico. Nos foruns e trabalho foi aconselhado não gravar todos os dados para a memória. Dessa forma realizamos a tarefa das duas formas e
 * 	deixo em comentario quais seriam as modificações no imprimir() sem a utilização de um vetor
 * 
 * Função void imprime(fstream& arq, int size);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- int size = dado retornado pela chamada do dbsize no loop principal, numero de filmes no nosso arquivo
 * 
 *  Como explicado acima, salva todo o conteudo do arquivo de uma vez em um vetor de buffer de filme e exibe cada um deles
 * 	por completo na tela para o usuário
 */
void imprime(fstream& arq, int size){
	//TO DO: imprimir listando vistos ou não vistos
	//cout << size << endl;
	
	//size = dbsize();
	cout << size << endl;
	filme buffer[size];
	
	arq.seekg (0);
	//leitura de todos os dados de uma vez
    arq.read ((char*) (buffer), size*sizeof(filme));
	
	for(int i=0;i<size;i++){
		cout << buffer[i].nome << endl;
		cout << buffer[i].ano << endl;
		cout << buffer[i].tipo << endl;
		cout << buffer[i].dura << endl;
		cout << buffer[i].dire << endl;
		cout << buffer[i].descri << endl;
		cout << buffer[i].aval << endl;
		cout << buffer[i].visto << endl;
	}
	
	/* void imprimir() sem vetor
	 * 
	 * filme buffer;
	 * arq.seekg(0);
	 * 
	 * for(int i=0;i<size;i++){
	 * 		arq.read ((char*) (&buffer), i*sizeof(filme));
	 *		cout << buffer.nome << endl;
	 *		cout << buffer.ano << endl;
	 *		cout << buffer.tipo << endl;
	 *		cout << buffer.dura << endl;
	 *		cout << buffer.dire << endl;
	 *		cout << buffer.descri << endl;
	 *		cout << buffer.aval << endl;
	 *		cout << buffer.visto << endl;
	 *	}
	 * 
	 */
}

/* Função void imprimeVisto(fstream& arq, int size, bool modo);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- int size = dado retornado pela chamada do dbsize no loop principal, numero de filmes no nosso arquivo
 *  - bool modo = dado referente a mostrar visto ou nao visto
 * 
 *  Le e salva um a um os dados dos filmes no arquivo em um unico buffer, itera pelo arquivo lendo pedaços com o tamanho relativo a um filme,
 *  checa qual o modo de impressão que o usuario quer e mostra os filmes vistos ou não vistos de acordo
 */
void imprimirVisto(fstream& arq, int size, bool modo){
	filme buffer;
	
	arq.seekg(0);
	cout << size << endl;
	for(int i=0;i<size;i++){
		arq.seekg(i*sizeof(filme));
		//le filme a filme de acordo com a posição do flag
		arq.read ((char*) (&buffer), sizeof(filme));
		
		//checa modo e se filme foi visto ou não para dar a saida correta
		if(modo){
			if(buffer.visto == 1){
				cout << buffer.nome << endl;
				cout << buffer.ano << endl;
				cout << buffer.tipo << endl;
				cout << buffer.dura << endl;
				cout << buffer.dire << endl;
				cout << buffer.descri << endl;
				cout << buffer.aval << endl;
				cout << buffer.visto << endl;
			}
		}
		
		if(!modo){
			if(buffer.visto == 0){
				cout << buffer.nome << endl;
				cout << buffer.ano << endl;
				cout << buffer.tipo << endl;
				cout << buffer.dura << endl;
				cout << buffer.dire << endl;
				cout << buffer.descri << endl;
				cout << buffer.aval << endl;
				cout << buffer.visto << endl;
			}
		}
	}
}

/* Função void imprimeUm(fstream& arq, int pos);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- int pos = dado retornado pela chamada do busca(), seja a busca chamada como parametro de imprimeUm(arq, busca(arq, size)) ou colocada em um aux
 *  
 *  Le e imprime todos os dados de um unico filme dado a posição onde ele se encontra no arquivo
 */
void imprimeUm(fstream& arq, int pos){
	
	filme buffer;
	//se achou a posição retornada na busca
	if(pos!= -1){
		//aponta o flag de leitura para a posição onde se encontra o filme
		arq.seekg(pos, arq.beg);
		arq.read((char*) (&buffer), sizeof(filme));
		
		cout << buffer.nome << endl;
		cout << buffer.ano << endl;
		cout << buffer.tipo << endl;
		cout << buffer.dura << endl;
		cout << buffer.dire << endl;
		cout << buffer.descri << endl;
		cout << buffer.aval << endl;
		cout << buffer.visto << endl;
		cout << endl;
		cout << "---Impressao concluida---" << endl;
	}
	else{
		cout << "Nao encontramos o filme buscado" << endl;
		cout << endl;
	}
}

/* Função int confirmNome(fstream& arq, int size, string k)
 * - fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * - int size = dado retornado pela chamada do busca()
 * - string k = string buscada, nome
 * 
 * Função que confirma se ja existe um nome similar no nosso arquivo
 */
int confirmNome(fstream& arq, int size, string k){
	int aux;
	filme buffer;
	
	//flag para leitura no inicio do arquivo
	arq.seekg (0);

	//multiplicamos a iteração pelo tamanho do filme para encontrar o ponto inicial de cada filme
	//salvamos o ponto de leitura no inicio da mesma com aux, lemos os dados e guardamos no nosso buffer
	//e comparamos o nome do buffer com a entrada do usuário
	for(int i=0;i<size;i++){
		arq.seekg(i*sizeof(filme),arq.beg);
		aux = arq.tellg();
		
		arq.read((char*)(&buffer), sizeof(filme));
		//key2 = buffer.nome;
		//retorna a posição se achou
		if(buffer.nome == k) return aux;	
	}
	//retorna -1 se não achou
	return -1;
}

/* Função void altera(fstream& arq, int pos);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- int pos = dado retornado pela chamada do busca(), seja a busca chamada como parametro de imprimeUm(arq, busca(arq, size)) ou colocada em um aux
 *  
 *  Altera o dado desejado de um filme que se encontra no arquivo.
 */
void altera(fstream &arq, int pos){ 
	//auxiliares para as entradas do usuário
	string snome;
	string stipo;
	string sdire;
	string sdescri;
	
	int aux;
	filme buffer;
	
	int size = dbsize(arq);
	
	int key = 1;
	
	
	arq.seekg(pos, arq.beg);
	arq.read((char*) (&buffer), sizeof(filme));
	
	//while de controle com switch case para o menu de opções do que alterar no filme
	while(key!=0){
		cout << " (1) Alterar nome do filme" << endl;
		cout << " (2) Alterar ano do filme" << endl;
		cout << " (3) Alterar tipo do filme" << endl;
		cout << " (4) Alterar duracao do filme" << endl;
		cout << " (5) Alterar diretor do filme" << endl;
		cout << " (6) Alterar avaliacao do filme" << endl;
		cout << " (7) Alterar descricao do filme" << endl;
		cout << " (8) Alterar se foi assistido ou nao" << endl;
		cout << " (9) Salvar alteracoes e Sair" << endl;
		cout << " (0) Sair e descartar alteracoes" << endl;
		cout << "Entre com a opcao desejada..." << endl;
		cin >> key;
		cout << endl;
		
		switch(key){
			case 1:
				cout << "Entre com o novo NOME do filme...(Ex: The Revenant)" << endl;	
				snome = getNome();
				strcpy(buffer.nome, snome.c_str());
				cout << "Voce digitou:  " << buffer.nome << endl;
			break;
				
			case 2:
				cout << "Entre com o novo ANO de lancamento do filme...(Ex: 2010)" << endl;
				buffer.ano = getAno();
				cout << "Voce digitou:  " << buffer.ano << endl;			
			break;
			
			case 3:
				cout << "Entre com o novo TIPO do filme...(Comedia, Drama, Ficcao)" << endl;
				stipo = getTipo();
				strcpy(buffer.tipo, stipo.c_str());
				cout << "Voce digitou:  " << buffer.tipo << endl;	
			break;
			
			case 4:
				cout << "Entre com a nova DURACAO do filme em minutos...(110)" << endl;
				buffer.dura = getDura();
				cout << "Voce digitou:  " << buffer.dura << endl;			
			break;
			
			case 5:
				cout << "Entre com o novo nome do DIRETOR do filme...(Quentin Tarantino)" << endl;
				sdire = getDire();
				strcpy(buffer.dire, sdire.c_str());
				cout << "Voce digitou:  " << buffer.dire << endl;
			break;
			
			case 6:
				cout << "Entre com a nova AVALIACAO do filme...(5.3)" << endl;
				buffer.aval = getAval();
				cout << "Voce digitou:  " << buffer.aval << endl;			
			break;
			
			case 7:
				cout << "Entre com a nova DESCRICAO do filme...(TBD por exemplo, poupar digitacao)" << endl;
				sdescri = getDescri();
				strcpy(buffer.descri, sdescri.c_str());
				cout << "Voce digitou:  " << buffer.descri << endl;
			break;
			
			case 8:
				cout << "Entre com filme VISTO ou nao. (0) para nao visto, (1) para visto..." << endl;
				buffer.visto = getVisto();
				cout << "Voce digitou:  " << buffer.visto << endl;			
			break;
			
			case 9:
				cout << "Salvando e saindo"<< endl;
				
				//chama função para confirmar se é possível fazer as alterações no arquivo com um nome que não seja repetido no arquivo
				aux = confirmNome(arq, size, snome);
				
				if(aux <0){
					arq.seekp(pos, arq.beg);
					arq.write((char*) (&buffer), sizeof(filme));
					key = 0;
				}
				else{
					cout << "Parece que ja existe um filme com esse nome no arquivo, altere o nome e tente novamente..." << endl;
				}
			break;
			
			case 0:
				cout << "Descartando e saindo"<< endl;
			break;
			
			default:
				cout << "Default"<< endl;
		}
	}
}

/* Função void shellSort(fstream& arq, int tam);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * 	- int tam = dado retornado pela chamada do dbsize no loop principal, numero de filmes no nosso arquivo (aqui utilizamos tam ao inves de size para indicar
 *  uma "mesma variavel utilizada anteriormente" pelo fato de ter muitos sizeof(filme) e facilitar a leitura e entendimento do codigo)
 *  
 *  Com a utilização de dois buffer compara os filmes que estão no nosso arquivo binário e os troca de posição utilizando o próprio arquivo quando é relevante
 * 	comentarios explicativos dentro da função
 */
void shellSort(fstream& arq, int tam){
	//shellSort com base nas avaliações
	
	//variáveis para iteração e para registrar posição do menor
	int i, j, menor;
	//como dito anteriormente, eu não estava conseguindo checar a comparação quando utilizando por exemplo, char key1[80]
	//se eu não associar o nome retirado do arquivo para uma variável string não parecia ser possível comparar as duas key1 == key2 ou > <
	//variaveis para a comparação de ordenamento
	string key1,key2;
	//buffers
	filme buffer, buffer2;
	
	arq.seekg(0);
	
	//iteração pelo arquivo até o penultimo membro, já que comparamos penultimo com ultimo 
	for(i=0;i<tam-1;i++){
		//guardamos posição do menor
		menor = i;
		
		//iteração pelo arquivo para ler o seguinte
		for(j=i+1;j<tam;j++){
			//posiciona flag de leitura para o proximo a ser comparado
			arq.seekg(j*sizeof(filme));
			//le proximo a ser comparado
			arq.read((char*) (&buffer), sizeof(filme));
			//variavel para comparação
			key1 = buffer.nome;
			
			//posiciona flag de leitura para o menor
			arq.seekg(menor*sizeof(filme));
			//le menor
			arq.read((char*) (&buffer2), sizeof(filme));
			key2 = buffer2.nome;
			
			//faz a comparação de nomes, nesse caso visando ordenar em ordem alfabética crescente
			//e realiza a troca do que esta sendo comparado com o menor se necessário
			if(key1 < key2){
				arq.seekp(menor*sizeof(filme));
				arq.write((char*) (&buffer), sizeof(filme));
				
				arq.seekp(j*sizeof(filme));
				arq.write((char*) (&buffer2), sizeof(filme));
			}
		}
	}
}

/* Função void adiciona(fstream& arq);
 * 	- fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 *  
 *  Com a utilização de dois buffer compara os filmes que estão no nosso arquivo binário e os troca de posição utilizando o próprio arquivo quando é relevante
 * 	comentarios explicativos dentro da função
 */
void adiciona(fstream& arq){
	//TODO::checkar entradas, comentários
	int aux=-1;
	int size;
	string snome;
	string stipo;
	string sdire;
	string sdescri;
	filme buffer;
	
	size = dbsize(arq);
	
	cout << "Vamos comecar a adicionar, note que aqui voce deve ficar atento a algumas coisas:" << endl;
	
	
	cout << endl;
	
	
	cout << "Entre com os dados quando for pedido" << endl;
	cout << endl;
	
		
	cout << "Entre com o NOME do filme...(Ex: The Revenant)" << endl;	
	
	snome = getNome();
	
	cout << "Voce digitou:  " << snome << endl;

	cout << "Checaremos se o filme ja existe" << endl;

	aux = confirmNome(arq, size, snome);
	
	if(aux>=0){
		cout << "Parece que o filme ja se encontra no arquivo, retornaremos ao menu..." << endl;
		//cin.ignore(0,'\n');
	}
	
	else{
		strcpy(buffer.nome, snome.c_str());
		
		cout << "Entre com o Ano de lancamento do filme...(Ex: 2010)" << endl;
		buffer.ano = getAno();
		cout << "Voce digitou:  " << buffer.ano << endl;
		
		cout << "Entre com o tipo do filme...(Comedia, Drama, Ficcao)" << endl;
		stipo = getTipo();
		strcpy(buffer.tipo, stipo.c_str());
		cout << "Voce digitou:  " << buffer.tipo << endl;
		
		cout << "Entre com a duracao do filme em minutos...(110)" << endl;
		buffer.dura = getDura();
		cout << "Voce digitou:  " << buffer.dura << endl;
		
		cout << "Entre com o nome do diretor do filme...(Quentin Tarantino)" << endl;
		sdire = getDire();
		strcpy(buffer.dire, sdire.c_str());
		cout << "Voce digitou:  " << buffer.dire << endl;
		
		cout << "Entre com avaliacao do filme...(5.3)" << endl;
		buffer.aval = getAval();
		cout << "Voce digitou:  " << buffer.aval << endl;
		
		cout << "Entre com a descricao do filme...(TBD por exemplo, poupar digitacao)" << endl;
		sdescri = getDescri();
		strcpy(buffer.descri, sdescri.c_str());
		cout << "Voce digitou:  " << buffer.descri << endl;
		
		cout << "Entre com filme visto ou não. (0) para não visto, (1) para visto..." << endl;
		buffer.visto = getVisto();
		cout << "Voce digitou:  " << buffer.visto << endl;
		
		arq.seekp(0, arq.end);
		arq.write((char*) (&buffer), sizeof(filme));
		
		cout << "Filme adicionado." << endl;
	}
}

/* Função void deleta(fstream& arq, int pos, int size)
 * - fstream& arq = ponteiro para o nosso arquivo aberto no loop principal para realização das operações
 * - int pos = dado retornado pela chamada do busca(), seja a busca chamada como parametro de imprimeUm(arq, busca(arq, size)) ou colocada em um aux
 * - int size = dado retornado pela chamada do busca()
 * 
 * Função para deletar um filme. Foi explicado no inicio do arquivo o motivo de manter ele.
 */
void deleta(fstream& arq, int pos, int size){
	
	string key;
	filme buffer;
	fstream arq_novo;

	
	
	arq.seekg(pos);
	arq.read((char*)(&buffer), sizeof(filme));
	key = buffer.nome;
	
	cout << key << endl;
	
	arq_novo.open("redeflix.dat.temp", ios::out | ios::binary);
	
	for(int i=0;i<size;i++){
		arq.seekg(i*sizeof(filme),arq.beg);
		arq.read((char*)(&buffer), sizeof(filme));
		
		if(buffer.nome != key){
			arq_novo.write((char*) (&buffer), sizeof(filme));
		}
	}
	
	arq.close();
	arq_novo.close();
	
	
	remove("redeflix.dat");
	rename("redeflix.dat.temp", "redeflix.dat");
		
}

/* Função void checkCliente();
 * 
 * Essa função é a primeira a ser chamada no nosso main(), ela tenta abrir o nosso arquivo. Caso não encontre nada
 * ela inicia os primeiros passos no programa, dependendo do que o usuário quer, criar um banco de dados do zero ou gerar o 
 * banco de dados que está disponível. Verificamos a entrada do usuário na escolha acima de acordo com tipo e o que é aceito, nesse caso sem
 * utilizar switch case(que também seria uma opção valida)
 */
void checkCliente(){
	fstream arq_db;
	//declara e fornece valor inadequado para variável de check de entrada de usuário
	int i=-1;
	
	//tenta achar e abrir o nosso arquivo
	arq_db.open("redeflix.dat", ios::in | ios::binary);
	
	//procedimentos para criação do primeiro arquivo binário (ou caso seja deletado)
	if(!arq_db){
		cout << "Hmm..... Parece que voce e novo aqui." << endl;
		cout << "O Redeflix se trata de um programa simples onde voce pode guardar as informacoes dos seus filmes favoritos." << endl;
		cout << "Pode marcar entre eles quais foram vistos, o que voce achou deles e por ai vai." << endl;
		cout << "Essa mensagem so aparece para voce que esta utilizando o 'aplicativo' pela primeira vez." << endl;
		cout << " (ou caso deletar o banco de dados ou algum erro com o mesmo) " << endl;
		cout << "Mas nao se preocupe, no menu principal voce podera ver as informacoes sobre o redeflix assim como algumas de suas funcionalidades e instrucoes." << endl;
		cout << "Verificamos e nao existe um banco de dados com os seus filmes." << endl;
		cout << "Por agora voce tem duas opcoes:"<< endl;
		cout << "( 1 ) Onde podemos criar um banco de dados com alguns filmes famosos para que voce possa ir direto a sua lista."<< endl;
		cout << "      (se voce escolher essa opçao, criaremos um arquivo com 20 filmes sem ordem especifica)" << endl;
		cout << "( 2 ) Ou voce pode comecar do zero e adicionar os seus propiors filmes. (esta opcao e mais trabalhosa)" << endl;
		cout << "Entre com sua opcao:  ";
		
		//check de valor e tipo da entrada do usuário (poderia ser switch case)
		while(i!=1 and i!=2){
			while(!(cin>>i)){
				cout << "Iii... parece que voce digitou algo que nao e um numero, tente novamente..." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			if(i!=1 and i!=2){
				cout << "Iii... parece que voce nao entendeu, precisamos que voce entre com (1) ou (2)" << endl;
				cout << "Tente de novo..." << endl;
			}
		}
	}
	
	arq_db.close();
	//de acordo com as opções do usuário da as respostas adequadas, ou se caso não achou, indica que ja é usuário do programa
	if(i==1){
		cout << "Voce digitou:" << i << endl;
		cout << "Criaremos o seu banco de dados." << endl; 
		//chamamos nossa função db() para criação do "banco de dados", nosso arquivo binário fornecido com dados
		db();
		cout << "Hmmm... parece que esta tudo certo, vamos te levaremos para o menu principal." << endl << endl;
	}
	else if(i==2){ 
		cout << "Voce digitou: " << i <<endl;
		cout << "Vamos te levar agora para o menu principal." << endl; 
		cout << "Certifique-se de comecar adicionando um filme" << endl << endl;
	}
	else{
		//segue o programa
		cout << "Parece que voce ja e nosso usuario, podemos ir direto para o menu principal." << endl << endl;
	}
}

void instrucao(){
	cout << "Na primeira utilização do programa e possível criar um arquivo vazio para começar a adicionar os filmes" << endl;
	cout << "ou deixar o redeflix criar o propiro arquivo com alguns filmes ja contidos nele" << endl;
	cout << "Verificado o arquivo que podemos trabalhar o usuário estará apto a começar a explorar as funcionalidades do programa." << endl;
	cout << "Escrevemos o programa para funcionar dentro de um loop que aciona um switch e case de menus e sub menus e so fecha o loop quando selecionado a saida e encerramento do mesmo." << endl;
	cout << "Podemos listar todos os filmes do nosso arquivo, podemos listar os filmes assistidos e podemos listar os filmes nao assistidos" << endl;
	cout << "Podemos buscar por um filme para lista-lo na tela ou alterar o seu conteudo, seja ele qual for." << endl;
	cout << "Podemos adicionar um filme novo ao nosso arquivo" << endl;
	cout << "Podemos ordenar o nosso arquivo" << endl;
	cout << "Podemos, por fim, excluir um filme do nosso arquivo" << endl;
	cout << "Alguns potnos sobre a utilizacao do programa" << endl;
	cout << "- Nosso identificador unico escolhido para verificacao no arquivo é o nome do filme, não é possivel ter dois filmes com o mesmo nome, assim como nao é possivel deletar um filme com nome nao encontrado no arquivo" << endl;
	cout << "- As entradas dadas pelo usuário tem de respeitar os limites e tipos:" << endl;
	cout << "    NOME de filme e string tamanho maximo 80 carateres (char[80])" << endl;
	cout << " 	 ANO do filme é do tipo inteiro (int)" << endl;
	cout << "    TIPO de filme e string tamanho maximo 80 carateres (char[80])" << endl;
	cout << " 	 DURACAO do filme é do tipo inteiro (int)" << endl;
	cout << "    DIRETOR de filme e string tamanho maximo 80 carateres (char[80])" << endl;
	cout << " 	 AVALIACAO do filme é do tipo ponto flutuante, pode ter ',' (float)" << endl;
	cout << "    DESCRICAO de filme e string tamanho maximo 200 carateres (char[200])" << endl;
	cout << " 	 VISTO do filme é do tipo inteiro (int)" << endl;
}

void sobre(){
	cout << "O redeflix.cpp é um programa que simula uma lista de filmes criada" << endl;
	cout << "e mantida pelo usuário utilizanto arquivos persistentes" << endl;
	cout << "Utilizamos arquivos binários para simular um banco de dados" << endl;
	cout << "onde ficam guardadas as informações pertinentes aos filmes que se encontram na lista" << endl;
	cout << "Cada filme (struct) é caracterizado por nome, ano de lançamento, tipo, duração," << endl;
	cout << "diretor do filme, descrição do filme, avaliação e se o mesmo já foi visto pelo usuário" << endl;
	cout << endl;
}

/* Função void loop();
 * 
 * Esse é o loop principal do nosso programa onde é exibido o menu e são chamadas todas as funções de acordo com
 * o que o usuário quer
 * 
 * A função é composta por um while em que dentro dele mostramos um menu e o gerimos por switch case onde só se sai do while com a opção
 * referente a SAIR do programa. Dentro de algumas opções do menu principal temos sub menus também geridos por switch case com opções de retornar ao menu principal
 * 
 * Dentro desse loop tentamos operar o máximo possível sem fechar o nosso arquivo aberto no começo do loop, mas em algumas das nossas funções se torna necessário fechar
 * este arquivo para operá-lo de alguma forma diferente, depois o reabrimos e continuamos com ele aberto até surgir outro caso em que se torna necessário operar de outra maneira,
 * caso contrário ele só é fechado quando saímos do programa
 * 
 * Da mesma forma, só chamamos nossa função dbsize() que nos retorna o numero de filmes que tem no arquivo no inicio do loop 
 * e só realizamos novamente quando necessário.
 */
void loop(){
	fstream arq;
	//variavel para retorno do dbsize() que é passada como referência em várias funções
	int size = 0;
	//key do controle do while/switch case externo, key2 do controle do switch case interno. Menu e sub menus
	int key=-1;
	int key2=0;
	//auxiliar para passagem de algumas posições
	int aux=0;
	int re=0;
	
	arq.open("redeflix.dat", ios::in | ios::out | ios::binary);
	
	if(!arq){
		cout << "Parece que ocorreu um erro abrindo arquivo..." << endl;
	}
	
	//entra no while do menu principal
	while(key!=0){
		
		if(!arq.is_open()){
			arq.open("redeflix.dat", ios::in | ios::out | ios::binary);
		}
		size = dbsize(arq);
		cout << size << endl;
		cout << " _______________________________________" << endl;
		cout << "| ------------MENU PRINCIPAL------------|" << endl;
		cout << "| ( 1 ) Listar filmes           --------|" << endl;
		cout << "| ( 2 ) Buscar/Alterar um filme --------|" << endl;
		cout << "| ( 3 ) Adicionar filme         --------|" << endl;
		cout << "| ( 4 ) Ordenar filmes          --------|" << endl;
		cout << "| ( 5 ) Excluir filme           --------|" << endl;
		cout << "| ( 6 ) Instrucoes              --------|" << endl;
		cout << "| ( 7 ) Sobre                   --------|" << endl;
		cout << "| ( 0 ) SAIR ---------------------------|" << endl;
		cout << "|_______________________________________|" << endl;
		cout << endl;
		cout << "Entre com a sua opcao....." << endl;
		
		cin >> key;
		cout << endl;
		
		switch(key){
		
			case 1:
				//sub menu com as opções de listar os filmes, controlado por switch case
				cout << endl;
				cout << "menu principal -> LISTAR FILMES" << endl;
				cout << " (1) Listar todos os filmes" << endl;
				cout << " (2) Listar filmes vistos" << endl;
				cout << " (3) Listar filmes nao vistos" << endl;
				cout << " (4) Voltar: MENU PRINCIPAL" << endl;
				cout << endl;
				cout << "Entre com a opcao....." << endl;
		
				cin >> key2;
				cout << endl;
				
				switch(key2){
					case 1:
						cout << "::LISTA DE TODOS OS FILMES::"<< endl;
						//chamada da função de impressão de todos os filmes
						imprime(arq, size);
						cout << "Impressao concluida." << endl;
						cout << "::VOLTANDO::" << endl;
					break;
					
					case 2:
						cout << "::LISTA DE TODOS OS FILMES VISTOS::"<< endl;
						//chamada da função de impressão de filmes vistos
						imprimirVisto(arq, size, true);
						cout << "::VOLTANDO::" << endl;
					break;
					
					case 3:
						cout << "::LISTA DE TODOS OS FILMES NAO VISTOS::"<< endl;
						//chamada da função de impressão de filmes nao vistos
						imprimirVisto(arq, size, false);
						cout << "::VOLTANDO::" << endl;
					break;
					
					case 4:
						cout << endl;
						cout << "::VOLTANDO::" << endl;
					break;
					
					default:
						cout << "Iii... parece que voce forneceu algo errado, tente de novo" << endl;
				}
			break;
			
			case 2:
				//sub menu com as opções de listar os filmes, controlado por switch case
				cout << endl;
				cout << "menu principal -> BUSCAR/ALTERAR UM FILME" << endl;
				cout << " (1) Buscar filme para visualizacao" << endl;
				cout << " (2) Alterar um filme" << endl;
				cout << " (3) Voltar: MENU PRINCIPAL" << endl;
				cout << endl;
				cout << "Entre com a opcao....." << endl;
		
				cin >> key2;
				cout << endl;
				
				switch(key2){
					/* Nos cases 1 e 2 começamos pela busca afim de imprimir o filme ou acha-lo para fazer alterações
					 * 
					 * Como se trata de um trabalho, eu acho importante ressaltar que é possível realizar algumas tarefas
					 * de formas diferentes para mostrar um pouco do conhecimento adquirido durante o período e
					 * as pesquisas relacionadas ao trabalho
					 * 
					 * No case 1, passamos a função busca() como parametro da função imprimeUm(), ja que um dos parametros
					 * de imprimeUm é um int e busca retorna um int. Caso não ache o filme buscado retorna ao menu principal
					 * 
					 * No case 2, optamos por dar a opção em forma de um while de o usuário re-entrar o nome do filme 
					 * para busca sem retornar ao menu principal, poupando o usuário de cliques desnecessários caso erre
					 * a entrada na hora de buscar. No case 1 optamos por mostrar algo mais teórico e no case 2 mostramos
					 * algo mais eficiente, porém, ambas poderiam ser escritas da mesma forma, e, ao meu ver, o "mais correto"
					 * nesse caso seria prover o usuário com a re-entrada dos dados afim de poupar tempo e clique, deixo em comentario
					 * como seria o case 1 dessa maneira
					 */ 
					case 1:
						cout << "::BUSCAR E IMPRIMIR FILME::"<< endl;
						//cout << "Me diga qual filme quer buscar..." << endl;
						//chamada da função de impressão de de um filme de acordo com o retorno da posição pela função busca(), nesse caso passado por parametro
						imprimeUm(arq, busca(arq, size));
						cout << "::VOLTANDO::" << endl;
						
						/* alem desse trecho de código teria que ser retirado o if..else case de dentro da função
						 * imprimeUm() essa parte não estará feito em comentario dentro da função
						while(aux == -1){
							cout << "Nao encontramos o filme buscado" << endl;
							cout << "Gostaria de tentar buscar novamente? Se sim digite 1, para não digite 2 ou qualquer outro numero pequeno" << endl;
							while(!(cin>>re)){
								cout << "Iii... parece que voce digitou algo que nao e um *INT*, tente novamente..." << endl;
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
							}
							cout << "Voce digitou:  " << re << endl;
							
							if(re == 1){
								cout << "Buscaremos novamente" << endl;
								aux = busca(arq, size);
							}
							else aux = -2;
						}
						if(aux>-1){
							cout << "Parece que achamos o filme buscado, de uma olhada..." << endl;
							imprimeUm(arq, aux);
							cout << "::VOLTANDO::" << endl;
						}
						*/
					break;
					
					case 2:
						cout << "::ALTERAR UM FILME::"<< endl;
						cout << "Primeiro me diga qual filme quer alterar..." << endl;
						//chamadda da função de que altera um filme, busca a posição do filme que tem que ser alterado
						//altera os dados e imprime com as alterações
						aux = busca(arq, size);
						
						while(aux == -1){
							cout << "Nao encontramos o filme buscado" << endl;
							cout << "Gostaria de tentar buscar novamente? Se sim digite 1, para nao digite 2 ou qualquer outro numero pequeno" << endl;
							while(!(cin>>re)){
								cout << "Iii... parece que voce digitou algo que nao e um *INT*, tente novamente..." << endl;
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
							}
							cout << "Voce digitou:  " << re << endl;
							
							if(re == 1){
								cout << "Buscaremos novamente" << endl;
								aux = busca(arq, size);
							}
							else aux = -2;
						}
						if(aux>-1){
							cout << "Parece que achamos o filme buscado, de uma olhada..." << endl;
							imprimeUm(arq, aux);
							altera(arq, aux);
							cout << "Parece que deu tudo certo, veja como ficou..." << endl;
							imprimeUm(arq, aux);
							cout << endl;
						}
						cout << "::VOLTANDO::" << endl;
					break;
						
					case 3:
						cout << endl;
						cout << "::VOLTANDO::" << endl;
					break;
					
					default:
						cout << "Iii... parece que voce forneceu algo errado, tente de novo" << endl;
							
				}
			break;
					
			case 3:
				//adição de filme, é necessário fechar nosso arquivo aberto no loop, reabrimos ele em modo ios::app
				//para adição no fim do arquivo, adiciona os dados, fecha o arq em modo app e o reabre para uso
				//dentro do loop
				cout << endl;
				cout << "menu principal -> ADICIONAR FILME" << endl;
				
				adiciona(arq);    
				
				
				cout << "::VOLTANDO::" << endl;		
			break;
					
			case 4:
				//ordena os filmes em ordem alfabética crescente
				cout << endl;
				cout << "menu principal -> ORDENAR FILMES" << endl;
				shellSort(arq, size);
				cout << "Filmes ordenados." << endl;
				cout << "::VOLTANDO::" << endl;	
			break;
			
			case 5:
				//exclui um filme
				cout << endl;
				cout << "menu principal -> EXCLUIR FILME" << endl;
				deleta(arq, busca(arq, size), size);         
				cout << "Filme excluido." << endl;
				cout << "::VOLTANDO::" << endl;
			break;
				
			case 6:
				cout << endl;
				cout << "menu principal -> ISNTRUCOES" << endl;
				instrucao();
				cout << "::VOLTANDO::" << endl;
				break;
			
			case 7:
				cout << endl;
				cout << "menu principal -> ISNTRUCOES" << endl;
				sobre();
				cout << "::VOLTANDO::" << endl;
			break;
				
			case 0:
				cout << endl;
				cout << "Esperamos o seu retorno" << endl;
				cout << "Tchau D=" << endl;
			break;
				
			default:	
				cout << "Iii... parece que voce forneceu algo errado, tente de novo" << endl;
		}
	}
	arq.close();
}

int main(){
	
	cout << "REDEFLIX" << endl;
	cout << endl;
	cout << endl;
	cout << "Bem vindo ao Redeflix." << endl << endl;
	
	checkCliente();
	
	loop();
	
	return 0;
}
