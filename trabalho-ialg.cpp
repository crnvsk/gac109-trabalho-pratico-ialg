#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

/*cadastro, alteração de dados, listagem de produtos disponíveis (por ordem alfabética
A,B,C...Z), visualização de nome de produto, realizar a venda de um produto e
exportação de produtos cadastrados com suas respectivas informações para um
arquivo binário.*/

struct Fornecedor;
struct Produto;
void saida(Produto produto);
void cadastrar();
void menu();
void consultar();

int main(){

    int key = -1;
    while(key != 8){
        menu();
        cin >> key;
        switch(key){
            case 1:
                //Cadastrar produto:
                cadastrar();
                break;
            case 2:
                //Consultar produto:
                consultar();
                break;
            case 3:
                //Listar produtos cadastrados disponíveis ordenados pelo código do produto:
                break;
            case 4:
                //Excluir um produto:
                break;
            case 5:
                //Efetuar uma venda:
                break;
            case 6:
                //Listar dados de produtos ativos em estoque:
                break;
            case 7:
                //Exportar dados de produtos ativos para arquivo binário:
                break;
            case 8:
                //Sair:
                cout << "Saindo...";
                break;
            default:
                break;
        }
    }

    return 0;
}

/*Descrição do produto: cadeira com rodízio XYZ
Código de identificação: CR123
Quantidade disponível: 25
Preço unitário: 346,80
Fornecedor:
Nome: MovEsc Ltda.
email: bel@beltrano.com bSilva@eletri.com.br
Telefone:+55 (35)38219997
situação: ativo*/

void menu(){
    cout << "\nEscolha o que deseja fazer:" << endl;
    cout << "(1) Cadastrar produto" << endl;
    cout << "(2) Consultar produto" << endl;
    cout << "(3) Listar produtos cadastrados disponiveis ordenados pelo codigo do produto" << endl;
    cout << "(4) Excluir um produto" << endl;
    cout << "(5) Efetuar uma venda" << endl;
    cout << "(6) Listar dados de produtos ativos em estoque" << endl;
    cout << "(7) Exportar dados de produtos ativos para arquivo binario" << endl;
    cout << "(8) Sair" << endl;
}
 
struct Fornecedor{
    char nome[60];
    int telefone;
    string email[3];
};

struct Produto{
    char descricao[60];
    char marca[20];
    char id[20];
    int quantidade;
    double preco;
    bool situacao;
    Fornecedor fornecedor;
};

void cadastrar(){
    Produto produto;
    string entrada;
    string str1 ("ativo");
    string str2 ("inativo");

    ofstream arquivo ("bancoDados.bin", ios::app);

    // Parte do Produto
    cout << "CADASTRO DE PRODUTO" << endl;
    cout << "Descricao do produto: ";
    cin.clear();
	while(cin.get() != '\n'){
        continue;
    }
    getline(std::cin, entrada);
    strcpy(produto.descricao, entrada.c_str());
    
    
    cout << "Marca do produto: ";
    cin.clear();
	
    getline(std::cin, entrada);
    strcpy(produto.marca, entrada.c_str());
    
    cout << "Codigo de identificacao: ";
    cin.clear();
	
    getline(std::cin, entrada);
    strcpy(produto.id, entrada.c_str());

    cout << "Quantidade disponivel: ";
    cin >> produto.quantidade;
    cout << "Preco unitario: ";
    cin >> produto.preco;

    // Parte do Fornecedor
    cout << "Nome do fornecedor: ";
    cin.clear();
	while(cin.get() != '\n'){
        continue;
    }
    getline(std::cin, entrada);
    strcpy(produto.fornecedor.nome, entrada.c_str());

    cout << "Quantos e-mails o Fornecedor possui? "; //consertar
    int chave;
    cin >> chave;
    int i = 0;
    while(i < chave){
        cout << "E-mail(" << i+1 << "): ";
        cin >> produto.fornecedor.email[i];
        i++;
    }

    cout << "Telefone: ";
    cin >> produto.fornecedor.telefone;
    

    cout << "Situacao(ativo ou inativo):  " << endl;
    chave = -1;
    while(chave == -1){
        cin >> entrada;
        if(entrada.compare(str1) == 0){
            produto.situacao = true;
            chave = 0;
        }
        else if (entrada.compare(str2) == 0){
            produto.situacao = false;
            chave = 0;
        }
    }
    
    arquivo.write((const char *)(&produto), sizeof(Produto));
    arquivo.close();
}

void consultar(){
    ifstream arquivo ("bancoDados.bin");
    Produto produto;
    arquivo.read((char *)(&produto), sizeof(Produto));
    cout << "Descricao do produto: ";
    cout << produto.descricao << endl;
    cout << "Marca do produto: ";
    cout << produto.marca << endl;
    cout << "Codigo de identificacao: ";
    cout << produto.id << endl;
    cout << "Quantidade disponivel: ";
    cout << produto.quantidade << endl;
    cout << "Preco unitario: ";
    cout << produto.preco << endl;
    cout << "Fornecedor: " << endl;
    cout << "Nome: ";
    cout << produto.fornecedor.nome << endl;

    cout << "E-mail(s) do Fornecedor: "; //consertar
    int i=0;
    while(i < 3){
        cout << produto.fornecedor.email[i] << endl;
        i++;
    }

    cout << "Telefone: ";
    cout << produto.fornecedor.telefone << endl;
    cout << "Situacao do produto: ";
    if(produto.situacao == true){
        cout << "Ativo" << endl;
    }
    else{
        cout << "Inativo" << endl;
    }
    arquivo.close();
}