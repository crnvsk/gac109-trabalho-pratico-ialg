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
    cout << "Descricao do produto: ";
    cout << produto.descricao << endl;
    cout << "Codigo de identificacao: ";
    cout << produto.id << endl;
    cout << "Quantidade disponivel: ";
    cout << produto.quantidade << endl;
    cout << "Preco unitario: ";
    cout << produto.preco << endl;
    cout << "Fornecedor: " << endl;
    cout << "Nome: ";
    cout << produto.fornecedor.nome << endl;

    cout << "E-mail(s) do Fornecedor: ";
    int i=0;
    while(i < 5){
        cout << produto.fornecedor.email[i] << endl;
        i++;
    }

    cout << "Telefone: ";
    cout << produto.fornecedor.telefone;

    cout << "situacao: ";
    cout << produto.situacao;
}

void cadastrar(){

    Produto produto;
    string entrada("");
    string str1 ("ativo");
    string str2 ("inativo");

    cout << "CADASTRO DE PRODUTO" << endl;
    cout << "Descricao do produto: ";
    cin >> produto.descricao;
    cout << "Codigo de identificacao:";
    cin >> produto.id;
    cout << "Quantidade disponivel:";
    cin >> produto.quantidade;
    cout << "Preco unitario: ";
    cin >> produto.preco;
    cout << "Nome do fornecedor: ";
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

    cout << "Situacaoo (ativo ou inativo):  " << endl;
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
    
    cout << "Escolha o que deseja fazer:" << endl;
    cout << "(1) Cadastrar produto" << endl;
    cout << "(2) Consultar produto" << endl;
    cout << "(3) Listar produtos cadastrados disponiveis ordenados pelo codigo do produto" << endl;
    cout << "(4) Excluir um produto" << endl;
    cout << "(5) Efetuar uma venda" << endl;
    cout << "(6) Listar dados de produtos ativos em estoque" << endl;
    cout << "(7) Exportar dados de produtos ativos para arquivo binario" << endl;
    cout << "(8) Sair" << endl;

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
            //Listar dados de produtos ativos em estoque:
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