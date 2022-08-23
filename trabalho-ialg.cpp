#include <iostream>
#include <string>

using namespace std;

/*Descrição do produto: cadeira com rodízio XYZ
Código de identificação: CR123
Quantidade disponível: 25
Preço unitário: 346,80
Fornecedor:
Nome: MovEsc Ltda.
email: bel@beltrano.com bSilva@eletri.com.br
Telefone:+55 (35)38219997
situação: ativo*/

struct Fornecedor{
    string nome;
    string email[5];
    string telefone;
};

struct Produto{
    string descricao;
    string id;
    int quantidade;
    double preco;
    bool situacao;
    Fornecedor fornecedor;
};

void saida(Produto produto){
    cout << "Descrição do produto: ";
    cout << produto.descricao << endl;
    cout << "Código de identificação:";
    cout << produto.id << endl;
    cout << "Quantidade disponível:";
    cout << produto.quantidade << endl;
    cout << "Preço unitário: ";
    cout << produto.preco << endl;
    cout << "Fornecedor: " << endl;
    cout << "Nome: " << endl;
    cout << produto.fornecedor.nome << endl;

    cout << "E-mail(s) do Fornecedor: ";
    int i=0;
    while(i < 5){
        cout << produto.fornecedor.email[i];
        i++;
    }

    cout << "Telefone: ";
    cout << produto.fornecedor.telefone;

    cout << "situação: ";
    cout << produto.situacao;
}

void cadastrar(){

    Produto produto;
    string entrada("");
    string str1 ("ativo");
    string str2 ("inativo");

    cout << "Descrição do produto: ";
    cin >> produto.descricao;
    cout << "Código de identificação:";
    cin >> produto.id;
    cout << "Quantidade disponível:";
    cin >> produto.quantidade;
    cout << "Preço unitário: ";
    cin >> produto.preco;
    cout << "Nome: ";
    cin >> produto.fornecedor.nome;

    cout << "Quantos e-mails o Fornecedor possui?(Maximo 5): ";
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

    cout << "Situação (ativo ou inativo):  " << endl;
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

    saida(produto);

}


int main(){

    /*Fornecedor semente;
    Produto maconha;

    cin >> semente.email[0];
    cout << semente.email[0];*/

    int key = -1;
    cin >> key;

    switch(key) {
        case 1:
            //Cadastrar produto:
            cadastrar();
            break;
        case 2:
            //Consultar produto:
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
            //Lstar dados de produtos ativos em estoque:
            break;
        case 7:
            //Exportar dados de produtos ativos para arquivo binário:
            break;
        case 8:
            //Sair:
            break;
        default:
            break;
    }

    return 0;
}