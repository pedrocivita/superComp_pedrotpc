#include <iostream>
#include <vector>
#include <string>

struct Item {
    std::string nome;
    int quantidade;
};

void adicionarItem(std::vector<Item>& estoque) {
    Item novo_item;
    std::cout << "Digite o nome do item: ";
    std::cin >> novo_item.nome;
    std::cout << "Digite a quantidade: ";
    std::cin >> novo_item.quantidade;
    estoque.push_back(novo_item);
    std::cout << "Item adicionado!" << std::endl;
}

void removerItem(std::vector<Item>& estoque) {
    std::string nome;
    std::cout << "Digite o nome do item a ser removido: ";
    std::cin >> nome;
    for(auto it = estoque.begin(); it != estoque.end(); ++it) {
        if(it->nome == nome) {
            estoque.erase(it);
            std::cout << "Item removido!" << std::endl;
            return;
        }
    }
    std::cout << "Item não encontrado!" << std::endl;
}

void visualizarEstoque(const std::vector<Item>& estoque) {
    std::cout << "Estoque atual:" << std::endl;
    for(const auto& item : estoque) {
        std::cout << "Nome: " << item.nome << ", Quantidade: " << item.quantidade << std::endl;
    }
}

int main() {
    std::vector<Item> estoque;
    int escolha;

    do {
        std::cout << "1. Adicionar item" << std::endl;
        std::cout << "2. Remover item" << std::endl;
        std::cout << "3. Visualizar estoque" << std::endl;
        std::cout << "4. Sair" << std::endl;
        std::cout << "Escolha uma opção: ";
        std::cin >> escolha;

        switch(escolha) {
            case 1: adicionarItem(estoque); break;
            case 2: removerItem(estoque); break;
            case 3: visualizarEstoque(estoque); break;
            case 4: std::cout << "Saindo..." << std::endl; break;
            default: std::cout << "Opção inválida!" << std::endl;
        }

    } while(escolha != 4);

    return 0;
}
