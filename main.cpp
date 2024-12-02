#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>
#include <functional>
#include <random>
#include <queue>
#include <limits>



using namespace std;

const int INF = numeric_limits<int>::max();


struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
};


TreeNode* insert(TreeNode* root, int value) {
    if (!root) return new TreeNode(value);
    if (value < root->value)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);
    return root;
}

void drawTree(TreeNode* root, sf::RenderWindow& window, sf::Font& font, float x, float y, float xOffset, float yOffset) {
    if (!root) return;

    sf::CircleShape node(20);
    node.setFillColor(sf::Color::White);
    node.setOutlineColor(sf::Color::Black);
    node.setOutlineThickness(2);
    node.setPosition(x - node.getRadius(), y - node.getRadius());
    window.draw(node);


    sf::Text valueText;
    valueText.setFont(font);
    valueText.setString(std::to_string(root->value));
    valueText.setCharacterSize(20);
    valueText.setFillColor(sf::Color::Black);
    valueText.setPosition(x - 10, y - 15); // Centrar texto
    window.draw(valueText);


    if (root->left) {
        sf::VertexArray edge(sf::Lines, 2);
        edge[0].position = sf::Vector2f(x, y);
        edge[1].position = sf::Vector2f(x - xOffset, y + yOffset);
        edge[0].color = edge[1].color = sf::Color::Black;
        window.draw(edge);
        drawTree(root->left, window, font, x - xOffset, y + yOffset, xOffset / 2, yOffset);
    }

    if (root->right) {
        sf::VertexArray edge(sf::Lines, 2);
        edge[0].position = sf::Vector2f(x, y);
        edge[1].position = sf::Vector2f(x + xOffset, y + yOffset);
        edge[0].color = edge[1].color = sf::Color::Black;
        window.draw(edge);
        drawTree(root->right, window, font, x + xOffset, y + yOffset, xOffset / 2, yOffset);
    }
}

pair<vector<int>,int> dijkstra(int n, int origen, const vector<vector<pair<int, int>>>& grafo, int destino) {

    vector<int> distancias(n, INF);
    vector<int> nodos(n, -1);
    distancias[origen] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, origen});

    while (!pq.empty()) {


        int distancia_actual = pq.top().first;
        int nodo_actual = pq.top().second;
        pq.pop();


        if (nodo_actual == destino) {
            break;
        }


        if (distancia_actual > distancias[nodo_actual]) continue;


        for (const auto& vecino : grafo[nodo_actual]) {
            int nodo_vecino = vecino.first;
            int peso_arista = vecino.second;


            int nueva_distancia = distancia_actual + peso_arista;


            if (nueva_distancia < distancias[nodo_vecino]) {
                distancias[nodo_vecino] = nueva_distancia;
                nodos[nodo_vecino] = nodo_actual;
                pq.push({nueva_distancia, nodo_vecino});

            }
        }
    }

    return {nodos,distancias[destino]};
}

vector<int> nodosDestino(vector<int>& node,int origen,int destino){

    vector<int> nodosUsados;
    int nodo = destino;
    while (nodo != -1) {
        nodosUsados.push_back(nodo);
        if (nodo == origen) {
            break;
        }
        nodo = node[nodo];
    }

    return nodosUsados;

}



struct Empleado {
    std::string nombre;
    std::string edad;
    std::string hobby;
    std::string lenguaje;
};


//#include <bits/stdc++.h>



int partition(vector<int> &vec, int low, int high) {


    int pivot = vec[high];

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    swap(vec[i + 1], vec[high]);

    return (i + 1);
}

void quickSort(vector<int> &vec, int low, int high) {

    if (low < high) {
        int pi = partition(vec, low, high);

        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}


void renderArray(sf::RenderWindow& window, sf::Sprite& background, std::vector<sf::Text>& numTexts) {
    window.clear();
    window.draw(background);

    for (const auto& text : numTexts) {
        window.draw(text);
    }

    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int inputNum(std::string a) {

    sf::RenderWindow window(sf::VideoMode(600, 200), a);


    sf::Font font;
    sf::Font font2;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        return 0;
    }

      if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }
    int num;

    sf::Text request;

    sf::Text textDisplay;
    textDisplay.setFont(font);
    textDisplay.setCharacterSize(30);
    textDisplay.setFillColor(sf::Color::White);
    textDisplay.setPosition(50, 50);


    request.setFont(font2);
    request.setString(a);
    request.setCharacterSize(15);
    request.setFillColor(sf::Color::White);
    request.setPosition(50, 20);

    std::string enteredText = "";


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {

                    if (event.text.unicode == 8 && !enteredText.empty()) {
                        enteredText.pop_back();
                    }

                    else {
                        enteredText += static_cast<char>(event.text.unicode);
                    }
                }
            }


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    num = std::stoi(enteredText);
                    return num;
                }
            }
        }


        textDisplay.setString(enteredText);


        window.clear();
        window.draw(textDisplay);
        window.display();
    }
    num = std::stoi(enteredText);
    return num;
}



void merge(const std::vector<int>& left, const std::vector<int>& right, std::vector<int>& merged,
           std::vector<sf::Text>& numTexts, sf::RenderWindow& window, sf::Sprite& background) {
    int i = 0, j = 0, k = 0;
     std::string x;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            merged[k] = left[i];
            numTexts[k].setString(std::to_string(left[i]));
             numTexts[k].setFillColor(sf::Color::Cyan);
             numTexts[k].setPosition(numTexts[k].getPosition().x ,150);
            i++;
        } else {
            merged[k] = right[j];
            numTexts[k].setString(std::to_string(right[j]));
            numTexts[k].setFillColor(sf::Color::Cyan);
            numTexts[k].setPosition(numTexts[k].getPosition().x ,150);
            j++;
        }
        k++;
    }

    while (i < left.size()) {
        merged[k] = left[i];
        sf::String sfStr = numTexts[k].getString();
        x = sfStr;

        numTexts[k].setString(std::to_string(left[i]));
        numTexts[k].setFillColor(sf::Color::Cyan);
        numTexts[k].setPosition(numTexts[k].getPosition().x ,150);
        i++;
        k++;
    }

    while (j < right.size()) {
        merged[k] = right[j];
       sf::String sfStr = numTexts[k].getString();
        x = sfStr;
        numTexts[k].setString(std::to_string(right[j]));

        numTexts[k].setFillColor(sf::Color::Cyan);
        numTexts[k].setPosition(numTexts[k].getPosition().x  ,150);
        j++;
        k++;
    }


    renderArray(window, background, numTexts);
    sf::sleep(sf::milliseconds(3000));

           }

void mergeSort(std::vector<int>& arrayx, std::vector<sf::Text>& numTexts, sf::RenderWindow& window, sf::Sprite& background) {
    int n = arrayx.size();
    if (n <= 1) return;

    int center = n / 2;


    std::vector<int> leftArray(center);
    std::vector<int> rightArray(n - center);

    for (int i = 0; i < n; i++) {
        if (i < center) {
            leftArray[i] = arrayx[i];
            numTexts[i].setFillColor(sf::Color::Red);

            numTexts[i].setFillColor(sf::Color::Red);
            numTexts[i].setPosition(numTexts[i].getPosition().x ,numTexts[i].getPosition().y+100);

        } else {
            rightArray[i - center] = arrayx[i];
            numTexts[i].setFillColor(sf::Color::Green);
            numTexts[i].setPosition(numTexts[i].getPosition().x ,numTexts[i].getPosition().y+100);
        }
    }


    renderArray(window, background, numTexts);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));


    mergeSort(leftArray, numTexts, window, background);
    mergeSort(rightArray, numTexts, window, background);


    merge(leftArray, rightArray, arrayx, numTexts, window, background);
}



int QuickSortW(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;


    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );



    int indexMENU = 0;
    int maxItemsMENU = 2;



    sf::Text MenuLS[2];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[0].setPosition(370, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Ordenar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[1].setPosition(470, 300);


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Quick Sort");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Magenta);
    instructions.setPosition(10, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 1) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 0) {
                int numSol = inputNum("Ingrese el numero de datos que desea ordenar");
                std::vector<int> vec;
                for (int i = 0; i < numSol; i++) {
                int nums = inputNum("Ingrese el numero " + std::to_string(i + 1));
                vec.push_back(nums);
                }

                 std::vector<sf::Text> numOrden(vec.size());
                for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                }

                BubbleSortWindow.clear();
                BubbleSortWindow.draw(SpriteFondo);
                BubbleSortWindow.draw(instructions);
                for (int i = 0; i < vec.size(); i++) {
                    BubbleSortWindow.draw(numOrden[i]);
                }

                BubbleSortWindow.display();

                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                quickSort(vec,0,vec.size()-1);

                 for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                }
                BubbleSortWindow.clear();
                BubbleSortWindow.draw(SpriteFondo);
                BubbleSortWindow.draw(instructions);
                for (int i = 0; i < vec.size(); i++) {
                    BubbleSortWindow.draw(numOrden[i]);
                }

                BubbleSortWindow.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                        }

                    }

                }
            }


        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }

        BubbleSortWindow.display();

                }


 return 0;


    }


int Stacks(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;
std::vector<sf::Text> numOrden(0);

    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexMENU = 0;
    int maxItemsMENU = 4;



    sf::Text MenuLS[4];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(sf::Color::Black);
    MenuLS[0].setPosition(270, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Push");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(sf::Color::Black);
    MenuLS[1].setPosition(370, 300);
    MenuLS[2].setFont(font);
    MenuLS[2].setString("Pop");
    MenuLS[2].setCharacterSize(20);
    MenuLS[2].setFillColor(sf::Color::Black);
    MenuLS[2].setPosition(470, 300);
     MenuLS[3].setFont(font);
    MenuLS[3].setString("Top");
    MenuLS[3].setCharacterSize(20);
    MenuLS[3].setFillColor(sf::Color::Black);
    MenuLS[3].setPosition(570, 300);


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Stacks");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(10, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 0) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 1) {
                    int x = inputNum("Ingrese el numero a agregar al stack");
                    vec.push_back(x);
                    numOrden.resize(vec.size());
                    for (int i = vec.size()-1; i >=0; i=i-1) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                }




                        }
                 if (indexMENU == 2) {


                if (!vec.empty()) {
                   vec.pop_back();
                    numOrden.resize(vec.size());
                    for (int i = vec.size()-1; i >=0; i=i-1) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);


                }
                } else {
                    cout<<"STACK VACIO"<<endl;
                }





                        }

                    if (indexMENU == 3) {


                 BubbleSortWindow.clear();
                BubbleSortWindow.draw(SpriteFondo);
                BubbleSortWindow.draw(instructions);

                                if (!vec.empty()) {
                    BubbleSortWindow.draw(numOrden[vec.size() - 1]);
                } else {
                    cout<<"STACK VACIO"<<endl;
                }





                BubbleSortWindow.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));


                        }

                    }

                }
            }


        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }
    for (int i = 0; i < vec.size(); i++) {
        BubbleSortWindow.draw(numOrden[i]);
    }


        BubbleSortWindow.display();

                }


 return 0;


    }

int drawBinaryTree() {
    sf::RenderWindow window(sf::VideoMode(960, 540), "Árbol Binario en SFML", sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }

    sf::Texture fondo;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Sprite SpriteFondo(fondo);
    sf::Vector2u windowSize = window.getSize();
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );

    TreeNode* root = nullptr;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 70);

    int indexMENU = 0;
    int maxItemsMENU = 2;
    sf::Text MenuOptions[2];
    MenuOptions[0].setFont(font);
    MenuOptions[0].setString("Regresar");
    MenuOptions[0].setCharacterSize(20);
    MenuOptions[0].setFillColor(sf::Color::Black);
    MenuOptions[0].setPosition(270, 300);

    MenuOptions[1].setFont(font);
    MenuOptions[1].setString("Agregar elemento");
    MenuOptions[1].setCharacterSize(20);
    MenuOptions[1].setFillColor(sf::Color::Black);
    MenuOptions[1].setPosition(370, 300);


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("BINARY TREE");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuOptions[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuOptions[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuOptions[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuOptions[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (indexMENU == 0) {

                        window.close();
                    }
                    if (indexMENU == 1) {

                        int newValue = inputNum("Ingresa un nuevo valor para el árbol:");
                        root = insert(root, newValue);
                    }
                }
            }
        }

        window.clear();
        window.draw(SpriteFondo);


        drawTree(root, window, font, 480, 50, 200, 80);


        for (int i = 0; i < maxItemsMENU; ++i) {
            window.draw(MenuOptions[i]);
        }
        window.draw(instructions);
        window.display();
    }

    return 0;
}



 int LinkedLists(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;
std::vector<sf::Text> numOrden(0);

    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexMENU = 0;
    int maxItemsMENU = 4;



    sf::Text MenuLS[4];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(sf::Color::Black);
    MenuLS[0].setPosition(270, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Quitar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(sf::Color::Black);
    MenuLS[1].setPosition(370, 300);
    MenuLS[2].setFont(font);
    MenuLS[2].setString("Agregar");
    MenuLS[2].setCharacterSize(20);
    MenuLS[2].setFillColor(sf::Color::Black);
    MenuLS[2].setPosition(470, 300);
     MenuLS[3].setFont(font);
    MenuLS[3].setString("Obtener");
    MenuLS[3].setCharacterSize(20);
    MenuLS[3].setFillColor(sf::Color::Black);
    MenuLS[3].setPosition(570, 300);

    sf::Text indexEl;
    indexEl.setFont(font);
                    indexEl.setCharacterSize(20);
                    indexEl.setColor(sf::Color::Black);
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Linked Lists");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(10, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black); // Restaurar color
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU; // Mover hacia arriba
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta); // Resaltar nueva opción
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 0) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 1) {
                    int x = inputNum("Ingrese el numero que desea eliminar");

                     auto it = find(vec.begin(), vec.end(), x);

                    if(it != vec.end()){
                         vec.erase(it);
                         numOrden.resize(vec.size());
                        for (int i = 0; i < vec.size(); i++) {
                        numOrden[i].setFont(font);
                        numOrden[i].setString(std::to_string(vec[i]));
                        numOrden[i].setCharacterSize(30);
                        numOrden[i].setFillColor(sf::Color::Magenta);
                        numOrden[i].setPosition(200 + (i * 50), 150);
                    }
                    }
                    else{
                        cout<<"NO EXISTE"<<endl;
                    }

                }





                 if (indexMENU == 2) {
                    int y = inputNum("Ingrese el número que desea agregar");
                    vec.push_back(y);
                    numOrden.resize(vec.size());
                     for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                }


                        }

                    if (indexMENU == 3) {

                int x = inputNum("INGRESE EL NUMERO QUE DESEA ENCONTRAR");

                auto it = find(vec.begin(), vec.end(), x);

                if (it != vec.end()) {
                    int index = distance(vec.begin(), it); // Calculate index

                    BubbleSortWindow.clear();
                    indexEl.setString(std::to_string(index));
                    indexEl.setPosition(numOrden[index].getPosition().x,210);

                    BubbleSortWindow.draw(SpriteFondo);
                    BubbleSortWindow.draw(indexEl);
                    BubbleSortWindow.draw(instructions);
                    BubbleSortWindow.draw(numOrden[index]);
                    BubbleSortWindow.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));


                } else {
                    cout << "NO ENCONTRADO" << endl;
                }

                 }
                        }

                    }

                }



        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }
     for (int i = 0;i<vec.size();i++) {
        BubbleSortWindow.draw(numOrden[i]);
    }


        BubbleSortWindow.display();

                }


 return 0;


    }





    int dijkstra2(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;
std::vector<sf::Text> numOrden(0);

    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }

    sf::Texture texturaImagen;
    if (!texturaImagen.loadFromFile("src/Assets/foto1.png")) {
    std::cerr << "Error al cargar la imagen.\n";
    return -1;


    }
    vector<int>nodosUsados;

    sf::Sprite spriteImagen;
    spriteImagen.setTexture(texturaImagen);
    spriteImagen.setPosition(500, 100);
    spriteImagen.setScale(0.5f, 0.5f);
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexMENU = 0;
    int maxItemsMENU = 2;



    sf::Text MenuLS[2];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(sf::Color::Black);
    MenuLS[0].setPosition(270, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Buscar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(sf::Color::Black);
    MenuLS[1].setPosition(370, 300);



              int n = 6;
    vector<vector<pair<int, int>>> grafo(n);
    int A=0;
    int B=1;
    int C=2;
    int D=3;
    int E=4;
    int F=5;
    grafo[A].push_back({B, 2120});
    grafo[A].push_back({C, 132});
    grafo[A].push_back({D, 1580});
    grafo[A].push_back({E, 1010});
    grafo[B].push_back({C, 2230});
    grafo[B].push_back({E, 1600});
    grafo[C].push_back({E, 799});
    grafo[C].push_back({F, 802});
    grafo[D].push_back({E, 535});
    grafo[E].push_back({F, 1300});
    //----------------------
    grafo[B].push_back({A, 2120});
    grafo[C].push_back({A, 132});
    grafo[D].push_back({A, 1580});
    grafo[E].push_back({A, 1010});
    grafo[C].push_back({B, 2230});
    grafo[E].push_back({B, 1600});
    grafo[E].push_back({C, 799});
    grafo[F].push_back({C, 802});
    grafo[E].push_back({D, 535});
    grafo[F].push_back({E, 1300});
    std::vector<sf::Text> nodosL(0);



    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Dijkstra");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);

     sf::Text distancia2;
    distancia2.setFont(font2);
    distancia2.setCharacterSize(20);
    distancia2.setFillColor(sf::Color::Black);
distancia2.setPosition(200, 200);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 0) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 1) {
                       int origen = inputNum("NODO DE PARTIDA");
                        int destino = inputNum("NODO DESTINO");
                        if(origen <= 5 && origen >=0 && destino <= 5 && destino >=0){
                        auto[nodos,distancia] = dijkstra(n,origen,grafo,destino);
                        vector<int> NODES = nodos;
                        nodosUsados=nodosDestino(NODES,origen,destino);


                        numOrden.resize(nodosUsados.size());
                        std::reverse(nodosUsados.begin(), nodosUsados.end());
                    for (int i = 0; i < nodosUsados.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(nodosUsados[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                    cout<<i<<endl;
                }
                distancia2.setString(std::to_string(distancia));


                }




                        }


                    }

                }
            }


        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        BubbleSortWindow.draw(spriteImagen);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }
     for (int i = 0;i<vec.size();i++) {
        BubbleSortWindow.draw(numOrden[i]);
    }
    for (int k = 0; k < nodosUsados.size(); k++) {
                BubbleSortWindow.draw(numOrden[k]);
            }
BubbleSortWindow.draw(distancia2);
        BubbleSortWindow.display();

                }


 return 0;


    }






int QueueAl(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;
std::vector<sf::Text> numOrden(0);

    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexMENU = 0;
    int maxItemsMENU = 4;



    sf::Text MenuLS[4];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(sf::Color::Black);
    MenuLS[0].setPosition(270, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Enqueue");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(sf::Color::Black);
    MenuLS[1].setPosition(370, 300);
    MenuLS[2].setFont(font);
    MenuLS[2].setString("Dequeue");
    MenuLS[2].setCharacterSize(20);
    MenuLS[2].setFillColor(sf::Color::Black);
    MenuLS[2].setPosition(470, 300);
     MenuLS[3].setFont(font);
    MenuLS[3].setString("Peek");
    MenuLS[3].setCharacterSize(20);
    MenuLS[3].setFillColor(sf::Color::Black);
    MenuLS[3].setPosition(570, 300);


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Queue");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(10, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 0) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 1) {
                    int x = inputNum("Ingrese el numero a agregar al queue");
                    vec.push_back(x);
                    numOrden.resize(vec.size());
                    for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                }




                        }
                 if (indexMENU == 2) {


                if (!vec.empty()) {
                     std::reverse(vec.begin(), vec.end());
                   vec.pop_back();
                 std::reverse(vec.begin(), vec.end());
                    numOrden.resize(vec.size());
                     for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);


                }
                } else {
                    cout<<"QUEUE VACIA"<<endl;
                }





                        }

                    if (indexMENU == 3) {


                 BubbleSortWindow.clear();
                BubbleSortWindow.draw(SpriteFondo);
                BubbleSortWindow.draw(instructions);

                                if (!vec.empty()) {
                    BubbleSortWindow.draw(numOrden[0]);
                } else {
                    cout<<"QUEUE VACIO"<<endl;
                }





                BubbleSortWindow.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));


                        }

                    }

                }
            }


        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }
     for (int i = 0;i<vec.size();i++) {
        BubbleSortWindow.draw(numOrden[i]);
    }


        BubbleSortWindow.display();

                }


 return 0;


    }



    int mergeSortW(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;


    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );



    int indexMENU = 0;
    int maxItemsMENU = 2;



    sf::Text MenuLS[2];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[0].setPosition(370, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Ordenar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[1].setPosition(470, 300);


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Merge Sort");
    instructions.setCharacterSize(40);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(10, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 1) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 0) {
                int numSol = inputNum("Ingrese el numero de datos que desea ordenar");
                std::vector<int> vec;
                for (int i = 0; i < numSol; i++) {
                int nums = inputNum("Ingrese el numero " + std::to_string(i + 1));
                vec.push_back(nums);
                }

                 std::vector<sf::Text> numOrden(vec.size());
                for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150);
                }


                mergeSort(vec, numOrden, BubbleSortWindow, SpriteFondo);

                        }

                    }

                }
            }


        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }





        BubbleSortWindow.display();

                }


 return 0;


    }


int BubbleSort(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<int> vec;


    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );



    int indexMENU = 0;
    int maxItemsMENU = 2;



    sf::Text MenuLS[2];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[0].setPosition(370, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Ordenar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[1].setPosition(470, 300);
    int x=0;
    int y=100;

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Bubble Sort");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 1) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 0) {
                int numSol = inputNum("Ingrese el numero de datos que desea ordenar");
                std::vector<int> vec;
                for (int i = 0; i < numSol; i++) {
                int nums = inputNum("Ingrese el numero " + std::to_string(i + 1));
                vec.push_back(nums);
                }

                sf::Text numOrden[vec.size()];
                for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150 );
                }

    for (int i = 0; i < vec.size() - 1; i++) {
        for (int j = 0; j < vec.size() - 1 - i; j++) {

            numOrden[j].setFillColor(sf::Color::Black);
            numOrden[j + 1].setFillColor(sf::Color::Black);

            BubbleSortWindow.clear();
            BubbleSortWindow.draw(SpriteFondo);
            for (int k = 0; k < vec.size(); k++) {
                BubbleSortWindow.draw(numOrden[k]);
            }
            BubbleSortWindow.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(1500));


            if (vec[j] > vec[j + 1]) {
                int temp = vec[j];
                vec[j] = vec[j+1];
                vec[j+1]=temp;
                numOrden[j].setString(std::to_string(vec[j]));
                numOrden[j + 1].setString(std::to_string(vec[j + 1]));
            }

            numOrden[j].setFillColor(sf::Color::Magenta);
            numOrden[j + 1].setFillColor(sf::Color::Magenta);
        }
    }



    BubbleSortWindow.clear();
    BubbleSortWindow.draw(SpriteFondo);
    for (int i = 0; i < vec.size(); i++) {
        BubbleSortWindow.draw(numOrden[i]);
    }
    BubbleSortWindow.draw(instructions);
    BubbleSortWindow.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
}

                    }


                }
            }


        BubbleSortWindow.clear();
        BubbleSortWindow.draw(SpriteFondo);
        BubbleSortWindow.draw(instructions);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }





        BubbleSortWindow.display();

                }


 return 0;


    }



int SelectionSort(){
    sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);



    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );



    int indexMENU = 0;
    int maxItemsMENU = 2;



    sf::Text MenuLS[2];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[0].setPosition(370, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Ordenar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[1].setPosition(470, 300);
    int x=0;
    int y=100;


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Selection Sort Visualization");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 1) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 0) {
                int numSol = inputNum("Ingrese el numero de datos que desea ordenar");
                std::vector<int> vec;
                for (int i = 0; i < numSol; i++) {
                int nums = inputNum("Ingrese el numero " + std::to_string(i + 1));
                vec.push_back(nums);
                }

                sf::Text numOrden[vec.size()];
                for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150 );
                }


        for (int i = 0; i < vec.size() ; i++) {
            int minVal = i;
                for(int j=i+1;j<vec.size() ;j++){
                numOrden[j-1].setFillColor(sf::Color::Magenta);
                numOrden[minVal].setFillColor(sf::Color::Green);
                numOrden[j].setFillColor(sf::Color::Black);

                    BubbleSortWindow.clear();
                    BubbleSortWindow.draw(SpriteFondo);
                    for (int k = 0; k < vec.size(); k++) {
                        BubbleSortWindow.draw(numOrden[k]);
                    }
                    BubbleSortWindow.display();

                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                            if(vec[j]<vec[minVal]){
                            minVal = j;


            }

        }
                int temp;
            if (minVal != i){

                temp = vec[i];
                vec[i]=vec[minVal];
                vec[minVal]= temp;
                numOrden[i].setString(std::to_string(vec[i]));
                numOrden[minVal].setString(std::to_string(vec[minVal]));


            }

            BubbleSortWindow.clear();
            BubbleSortWindow.draw(SpriteFondo);
            for (int i = 0; i < vec.size(); i++) {
                numOrden[i].setFillColor(sf::Color::Magenta);
                BubbleSortWindow.draw(numOrden[i]);
            }
            BubbleSortWindow.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(4000));

            }

                    }


                }
            }




                }

        BubbleSortWindow.draw(SpriteFondo);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }


        BubbleSortWindow.draw(instructions);


        BubbleSortWindow.display();




    }

    return 0;
}

 int InsertionSort(){
        sf::RenderWindow BubbleSortWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);



    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BubbleSortWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );



    int indexMENU = 0;
    int maxItemsMENU = 2;



    sf::Text MenuLS[2];

    MenuLS[0].setFont(font);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[0].setPosition(370, 300);
    MenuLS[1].setFont(font);
    MenuLS[1].setString("Ordenar");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[1].setPosition(470, 300);
    int x=0;
    int y=100;


    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Insertion Sort");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);





     while (BubbleSortWindow.isOpen()) {
        sf::Event event;
        while (BubbleSortWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BubbleSortWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black); // Restaurar color
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU; // Mover hacia arriba
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta); // Resaltar nueva opción
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 1) {

                        BubbleSortWindow.close();

                    }
                     if (indexMENU == 0) {
                int numSol = inputNum("Ingrese el numero de datos que desea ordenar");
                std::vector<int> vec;
                for (int i = 0; i < numSol; i++) {
                int nums = inputNum("Ingrese el numero " + std::to_string(i + 1));
                vec.push_back(nums);
                }

                sf::Text numOrden[vec.size()];
                for (int i = 0; i < vec.size(); i++) {
                    numOrden[i].setFont(font);
                    numOrden[i].setString(std::to_string(vec[i]));
                    numOrden[i].setCharacterSize(30);
                    numOrden[i].setFillColor(sf::Color::Magenta);
                    numOrden[i].setPosition(200 + (i * 50), 150 );
                }



        for (int i = 1; i < vec.size() ; i++) {
            int key = vec[i];
            int j = i-1;

            numOrden[i].setFillColor(sf::Color::Black);

            BubbleSortWindow.clear();
            BubbleSortWindow.draw(SpriteFondo);
            for (int k = 0; k < vec.size(); k++) {
                BubbleSortWindow.draw(numOrden[k]);
            }
            BubbleSortWindow.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(1500));


            int temp;
            while (j>=0 && vec[j]>key){
            numOrden[j].setFillColor(sf::Color::Blue);
            numOrden[j + 1].setFillColor(sf::Color::Blue);

             vec[j+1] = vec[j];
             numOrden[j + 1].setString(std::to_string(vec[j]));
             numOrden[j].setString(std::to_string(key));
             j=j-1;
             numOrden[j + 1].setFillColor(sf::Color::Magenta);

            BubbleSortWindow.clear();
            BubbleSortWindow.draw(SpriteFondo);


            for (int k = 0; k < vec.size(); k++) {
                numOrden[k].setFillColor(sf::Color::Magenta);
                BubbleSortWindow.draw(numOrden[k]);
            }
            BubbleSortWindow.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(700));

            }

            vec[j + 1] = key;
            numOrden[j + 1].setString(std::to_string(key));
            numOrden[j + 1].setFillColor(sf::Color::Green);
            numOrden[i].setFillColor(sf::Color::Magenta);

             BubbleSortWindow.clear();
            BubbleSortWindow.draw(SpriteFondo);
            for (int k = 0; k < vec.size(); k++) {
                BubbleSortWindow.draw(numOrden[k]);
            }
            BubbleSortWindow.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        }



    BubbleSortWindow.clear();
    BubbleSortWindow.draw(SpriteFondo);

    for (int i = 0; i < vec.size(); i++) {
        numOrden[i].setFillColor(sf::Color::Magenta);
        BubbleSortWindow.draw(numOrden[i]);
    }
    BubbleSortWindow.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

}

                    }


                }
            }



        BubbleSortWindow.draw(SpriteFondo);
        for (int i = 0; i < maxItemsMENU; i++) {
            BubbleSortWindow.draw(MenuLS[i]);
        }

        BubbleSortWindow.draw(instructions);



        BubbleSortWindow.display();

                }


 return 0;


    }


bool comparacion(Empleado _a , Empleado _b) {
    return _a.nombre < _b.nombre;
}

std::string inputText(std::string a) {
    // Crear una ventana
    sf::RenderWindow window(sf::VideoMode(600, 200), "Ingrese el nombre");

    // Cargar la fuente
    sf::Font font;
    sf::Font font2;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        return "";
    }

     if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return "";
    }

    // Configurar el texto para mostrar lo que el usuario escribe
    sf::Text textDisplay;

    sf::Text request;

    textDisplay.setFont(font);
    textDisplay.setCharacterSize(30);
    textDisplay.setFillColor(sf::Color::White);
    textDisplay.setPosition(50, 50);

    request.setFont(font2);
    request.setString(a);
    request.setCharacterSize(15);
    request.setFillColor(sf::Color::White);
    request.setPosition(50, 20);

    std::string enteredText = "";

    // Bucle principal de la ventana
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Cerrar la ventana
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Manejar la entrada de texto
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    // Si se presiona la tecla backspace, eliminar el último carácter
                    if (event.text.unicode == 8 && !enteredText.empty()) {
                        enteredText.pop_back();
                    }
                    // Si es otro carácter, agregarlo al texto ingresado
                    else {
                        enteredText += static_cast<char>(event.text.unicode);
                    }
                }
            }

            // Verificar si se presionó la tecla B
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    return enteredText; // Regresar el texto cuando se presiona B
                }
            }
        }

        // Actualizar el texto mostrado en la ventana
        textDisplay.setString(enteredText);

        // Limpiar la ventana y dibujar el texto
        window.clear();
        window.draw(textDisplay);
        window.display();
    }

    return enteredText; // Regresar el texto cuando la ventana se cierra (aunque no se espera que esto ocurra)
}


int linearSearch(){

 sf::RenderWindow linearSearchWindow(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<Empleado> empleados = {
        {"Uri", "31", "Natación, Jardinería", "Cloudflare"},
        {"Vince", "28", "Leer, Escalar", "Fastly"},
        {"Willa", "30", "Yoga, Cine", "Kong"},
        {"Ximena", "27", "Correr, Pintura", "Nginx"},
        {"Yusuf", "29", "Ciclismo, Jardinería", "HAProxy"},
        {"Zoe", "26", "Natación, Fotografía", "Consul"},
        {"Andre", "31", "Escalar, Ajedrez", "Vault"},
        {"Beth", "28", "Leer, Videojuegos", "Prometheus"},
        {"Chris", "35", "Correr, Música", "Grafana"},
        {"Daisy", "24", "Natación, Escalar", "Kibana"},
        {"Ethan", "27", "Fotografía, Ciclismo", "ElasticSearch"},
        {"Faith", "29", "Teatro, Piano", "Logstash"},
        {"Gabe", "30", "Cocina, Leer", "Fluentd"},
        {"Hazel", "32", "Ajedrez, Jardinería", "Kafka"},
        {"Ian", "28", "Correr, Yoga", "RabbitMQ"},
        {"Judy", "26", "Videojuegos, Fotografía", "ActiveMQ"},
        {"Karl", "34", "Ciclismo, Programar", "Kafka Streams"},
        {"Lena", "29", "Escalar, Cine", "Spark Streaming"},
        {"Mick", "31", "Leer, Música", "Flink"},
        {"Nina", "27", "Pintura, Jardinería", "Samza"},
        {"Fiona", "27", "Cocina, Viajar", "Racket"}};

    sort(empleados.begin(),empleados.end(),comparacion);
    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }



    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = linearSearchWindow.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexBS = 0;
    int maxItemsBS = empleados.size();

    int indexMENU = 0;
    int maxItemsMENU = 2;
    int mid = 0;
     sf::Text persona[4];

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Linear Search");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);

    sf::Text empleadosLista[empleados.size()];
    sf::Text MenuLS[2];

    MenuLS[0].setFont(font2);
    MenuLS[0].setString("Regresar");
    MenuLS[0].setCharacterSize(20);
    MenuLS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[0].setPosition(550, 300);
    MenuLS[1].setFont(font2);
    MenuLS[1].setString("Realizar busqueda");
    MenuLS[1].setCharacterSize(20);
    MenuLS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuLS[1].setPosition(650, 300);
    int x=0;
    int y=100;
    for(i=0;i<empleados.size()-1;i++){
    if(i%5 == 0){
        y = y + 50;
       x=0;
    }
    empleadosLista[i].setFont(font2);
    empleadosLista[i].setString(empleados[i].nombre);
    empleadosLista[i].setCharacterSize(15);
    empleadosLista[i].setFillColor(0 == mid ? sf::Color::Magenta : sf::Color::Black);
    empleadosLista[i].setPosition(200+(x*50), y);
    x=x+1;
    }

    bool personFound = false;



     while (linearSearchWindow.isOpen()) {
        sf::Event event;
        while (linearSearchWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                linearSearchWindow.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black); // Restaurar color
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU; // Mover hacia arriba
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta); // Resaltar nueva opción
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuLS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuLS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {

                    if (indexMENU == 1) {

                        linearSearchWindow.close();

                    }
                     if (indexMENU == 0) {

                        std::string NAME=inputText("Ingrese el nombre a buscar");
                        if (NAME.empty()) {
                            std::cout << "No input provided" << std::endl;
                        }
                        else {
                            std::cout << "Entered text: " << NAME << std::endl;
                        }


                            int x=0;
                         for(i=0;i<empleados.size();i++) {
                            empleadosLista[x].setFillColor(sf::Color::Magenta);
                            x=i;

                            if (empleados[i].nombre == NAME) {

                                std::cout << empleados[i].nombre << std::endl;
                                empleadosLista[i].setFillColor(sf::Color::Red);
                                persona[0].setFont(font);
                                persona[0].setString("Name: " + empleados[i].nombre);
                                persona[0].setCharacterSize(20);
                                persona[0].setFillColor(sf::Color::Magenta);
                                persona[0].setPosition(200, 420);

                                persona[1].setFont(font);
                                persona[1].setString("Age: " + empleados[i].edad);
                                persona[1].setCharacterSize(20);
                                persona[1].setFillColor(sf::Color::Magenta);
                                persona[1].setPosition(320, 420);  // Position below the name

                                persona[2].setFont(font);
                                persona[2].setString("Hobby: " + empleados[i].hobby);
                                persona[2].setCharacterSize(20);
                                persona[2].setFillColor(sf::Color::Magenta);
                                persona[2].setPosition(390, 420);  // Position below age

                                persona[3].setFont(font);
                                persona[3].setString("Language: " + empleados[i].lenguaje);
                                persona[3].setCharacterSize(20);
                                persona[3].setFillColor(sf::Color::Magenta);
                                persona[3].setPosition(650, 420);  // Position below hobby

                                personFound = true;

                                linearSearchWindow.clear();
                                linearSearchWindow.draw(SpriteFondo);
                                    for (int i = 0; i < maxItemsBS; i++) {
                                        linearSearchWindow.draw(empleadosLista[i]);
                                    }


                              linearSearchWindow.display();

                              std::this_thread::sleep_for(std::chrono::seconds(2));


                                break;
                            }

                            linearSearchWindow.clear();
                            linearSearchWindow.draw(SpriteFondo);
                            empleadosLista[i].setFillColor(sf::Color::Black);
                                    for (int i = 0; i < maxItemsBS; i++) {
                                        linearSearchWindow.draw(empleadosLista[i]);
                                    }


                                    linearSearchWindow.display();

                              std::this_thread::sleep_for(std::chrono::seconds(2));


                         }



                        }




                        }


                }
                }

        empleadosLista[i].setFillColor(sf::Color::Magenta);
        linearSearchWindow.clear();
        linearSearchWindow.draw(SpriteFondo);
        for (int i = 0; i < maxItemsMENU; i++) {
            linearSearchWindow.draw(MenuLS[i]);
        }

        for (int i = 0; i < maxItemsBS; i++) {
            linearSearchWindow.draw(empleadosLista[i]);
        }


        if (personFound) {
            for (int i = 0; i < 4; i++) {
                linearSearchWindow.draw(persona[i]);
            }
        }

        linearSearchWindow.draw(instructions);
        linearSearchWindow.display();

                }








 return 0;
    }


int binarySearch(){

 sf::RenderWindow BinarySearch(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);
 std::vector<Empleado> empleados = {
        {"Uri", "31", "Natación, Jardinería", "Cloudflare"},
        {"Vince", "28", "Leer, Escalar", "Fastly"},
        {"Willa", "30", "Yoga, Cine", "Kong"},
        {"Ximena", "27", "Correr, Pintura", "Nginx"},
        {"Yusuf", "29", "Ciclismo, Jardinería", "HAProxy"},
        {"Zoe", "26", "Natación, Fotografía", "Consul"},
        {"Andre", "31", "Escalar, Ajedrez", "Vault"},
        {"Beth", "28", "Leer, Videojuegos", "Prometheus"},
        {"Chris", "35", "Correr, Música", "Grafana"},
        {"Daisy", "24", "Natación, Escalar", "Kibana"},
        {"Ethan", "27", "Fotografía, Ciclismo", "ElasticSearch"},
        {"Faith", "29", "Teatro, Piano", "Logstash"},
        {"Gabe", "30", "Cocina, Leer", "Fluentd"},
        {"Hazel", "32", "Ajedrez, Jardinería", "Kafka"},
        {"Ian", "28", "Correr, Yoga", "RabbitMQ"},
        {"Judy", "26", "Videojuegos, Fotografía", "ActiveMQ"},
        {"Karl", "34", "Ciclismo, Programar", "Kafka Streams"},
        {"Lena", "29", "Escalar, Cine", "Spark Streaming"},
        {"Mick", "31", "Leer, Música", "Flink"},
        {"Nina", "27", "Pintura, Jardinería", "Samza"},
        {"Fiona", "27", "Cocina, Viajar", "Racket"}};

    sort(empleados.begin(),empleados.end(),comparacion);
    int i;

    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = BinarySearch.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexBS = 0;
    int maxItemsBS = empleados.size();

    int indexMENU = 0;
    int maxItemsMENU = 2;
    int mid = 0;
     sf::Text persona[4];

     sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Binary Search");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(100, 10);

    sf::Text empleadosLista[empleados.size()];
    sf::Text MenuBS[2];

    MenuBS[0].setFont(font2);
    MenuBS[0].setString("Regresar");
    MenuBS[0].setCharacterSize(20);
    MenuBS[0].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuBS[0].setPosition(550, 300);
    MenuBS[1].setFont(font2);
    MenuBS[1].setString("Realizar busqueda");
    MenuBS[1].setCharacterSize(20);
    MenuBS[1].setFillColor(0 == indexMENU ? sf::Color::Magenta : sf::Color::Black);
    MenuBS[1].setPosition(650, 300);
    int x=0;
    int y=100;
    for(i=0;i<empleados.size()-1;i++){
    if(i%5 == 0){
        y = y + 50;
       x=0;
    }
    empleadosLista[i].setFont(font2);
    empleadosLista[i].setString(empleados[i].nombre);
    empleadosLista[i].setCharacterSize(15);
    empleadosLista[i].setFillColor(0 == mid ? sf::Color::Magenta : sf::Color::Black);
    empleadosLista[i].setPosition(200+(x*50), y);
    x=x+1;
    }

    bool personFound = false;



     while (BinarySearch.isOpen()) {
        sf::Event event;
        while (BinarySearch.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                BinarySearch.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    MenuBS[indexMENU].setFillColor(sf::Color::Black); // Restaurar color
                    indexMENU = (indexMENU - 1 + maxItemsMENU) % maxItemsMENU; // Mover hacia arriba
                    MenuBS[indexMENU].setFillColor(sf::Color::Magenta); // Resaltar nueva opción
                }
                if (event.key.code == sf::Keyboard::Right) {
                    MenuBS[indexMENU].setFillColor(sf::Color::Black);
                    indexMENU = (indexMENU + 1) % maxItemsMENU;
                    MenuBS[indexMENU].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << MenuBS[indexMENU].getString().toAnsiString() << "\n";

                    if (indexMENU == 1) {

                        BinarySearch.close();

                    }
                     if (indexMENU == 0) {

                        std::string NAME=inputText("Ingrese el nombre a buscar");
                        if (NAME.empty()) {
                            std::cout << "No input provided" << std::endl;
                        }
                        else {
                            std::cout << "Entered text: " << NAME << std::endl;
                        }

                        int izq = 0;
                        int der = empleados.size() - 1;

                        while (izq <= der) {
                            empleadosLista[mid].setFillColor(sf::Color::Magenta);
                            mid = izq + (der - izq) / 2;




                            if (empleados[mid].nombre == NAME) {

                                std::cout << empleados[mid].nombre << std::endl;
                                empleadosLista[mid].setFillColor(sf::Color::Red);
                                persona[0].setFont(font);
                                persona[0].setString("Name: " + empleados[mid].nombre);
                                persona[0].setCharacterSize(20);
                                persona[0].setFillColor(sf::Color::Magenta);
                                persona[0].setPosition(200, 420);

                                persona[1].setFont(font);
                                persona[1].setString("Age: " + empleados[mid].edad);
                                persona[1].setCharacterSize(20);
                                persona[1].setFillColor(sf::Color::Magenta);
                                persona[1].setPosition(320, 420);  // Position below the name

                                persona[2].setFont(font);
                                persona[2].setString("Hobby: " + empleados[mid].hobby);
                                persona[2].setCharacterSize(20);
                                persona[2].setFillColor(sf::Color::Magenta);
                                persona[2].setPosition(390, 420);  // Position below age

                                persona[3].setFont(font);
                                persona[3].setString("Language: " + empleados[mid].lenguaje);
                                persona[3].setCharacterSize(20);
                                persona[3].setFillColor(sf::Color::Magenta);
                                persona[3].setPosition(650, 420);  // Position below hobby

                                personFound = true;

                                break;
                            }

                            if (empleados[mid].nombre < NAME) {

                                izq = mid + 1;
                            } else {
                                der = mid - 1;
                            }

                            BinarySearch.clear();
                                    BinarySearch.draw(SpriteFondo);

                                    empleadosLista[mid].setFillColor(sf::Color::Black);
                                    for (int i = 0; i < maxItemsBS; i++) {
                                        BinarySearch.draw(empleadosLista[i]);
                                    }


                                    BinarySearch.display();

                              std::this_thread::sleep_for(std::chrono::seconds(2));

                        }




                        }


                }
                }
                }


        int mid2=0;





        BinarySearch.clear();
        BinarySearch.draw(SpriteFondo);
        for (int i = 0; i < maxItemsMENU; i++) {
            BinarySearch.draw(MenuBS[i]);
        }

        for (int i = 0; i < maxItemsBS; i++) {
            BinarySearch.draw(empleadosLista[i]);
        }


        if (personFound) {
            for (int i = 0; i < 4; i++) {
                BinarySearch.draw(persona[i]);
            }
        }
        BinarySearch.draw(instructions);
        BinarySearch.display();


    }
    return 0;
}


int ORDENAMIENTO(){

     sf::RenderWindow windowBusqueda(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);


    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = windowBusqueda.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexO = 0;
    int maxItemsO = 6;
    sf::Text menuBusqueda[6];

    menuBusqueda[0].setFont(font2);
    menuBusqueda[0].setString("Bubble Sort");
    menuBusqueda[0].setCharacterSize(36);
    menuBusqueda[0].setFillColor(0 == indexO ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[0].setPosition(275, 150);

    menuBusqueda[1].setFont(font2);
    menuBusqueda[1].setString("Insertion Sort");
    menuBusqueda[1].setCharacterSize(36);
    menuBusqueda[1].setFillColor(1 == indexO ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[1].setPosition(275, 200);

    menuBusqueda[2].setFont(font2);
    menuBusqueda[2].setString("Selection Sort");
    menuBusqueda[2].setCharacterSize(36);
    menuBusqueda[2].setFillColor(2 == indexO ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[2].setPosition(275, 250);

    menuBusqueda[3].setFont(font2);
    menuBusqueda[3].setString("Merge Sort");
    menuBusqueda[3].setCharacterSize(36);
    menuBusqueda[3].setFillColor(0 == indexO ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[3].setPosition(275, 300);

    menuBusqueda[4].setFont(font2);
    menuBusqueda[4].setString("Quick Sort");
    menuBusqueda[4].setCharacterSize(36);
    menuBusqueda[4].setFillColor(1 == indexO ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[4].setPosition(275, 350);

    menuBusqueda[5].setFont(font2);
    menuBusqueda[5].setString("Regresar");
    menuBusqueda[5].setCharacterSize(36);
    menuBusqueda[5].setFillColor(2 == indexO ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[5].setPosition(275, 400);




    while (windowBusqueda.isOpen()) {
        sf::Event event;
        while (windowBusqueda.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowBusqueda.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuBusqueda[indexO].setFillColor(sf::Color::Black); // Restaurar color
                    indexO = (indexO - 1 + maxItemsO) % maxItemsO; // Mover hacia arriba
                    menuBusqueda[indexO].setFillColor(sf::Color::Magenta); // Resaltar nueva opción
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menuBusqueda[indexO].setFillColor(sf::Color::Black);
                    indexO = (indexO + 1) % maxItemsO; // Mover hacia abajo
                    menuBusqueda[indexO].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << menuBusqueda[indexO].getString().toAnsiString() << "\n";

                    if (indexO == 5) {

                        windowBusqueda.close();

                    }
                    if (indexO == 4) {
                    QuickSortW();
                    }
                      if (indexO == 3) {
                        mergeSortW();
                    }
                    if (indexO == 2) {
                        SelectionSort();
                    }

                      if (indexO == 1) {
                        InsertionSort();
                    }
                     if (indexO == 0) {
                        BubbleSort();
                    }




                }
                }
                }

                windowBusqueda.clear();
        windowBusqueda.draw(SpriteFondo);
        for (int i = 0; i < maxItemsO; i++) {
            windowBusqueda.draw(menuBusqueda[i]);
        }
        windowBusqueda.display();
    }
    return 0;




}

class Maze {
public:
    Maze(int width, int height, int cellSize);
    void generateMaze();
    bool solveMaze(sf::RenderWindow& window);  // Declaración del método solveMaze
    bool solveMazeBFS(sf::RenderWindow& window);  // Método para resolver con BFS
    void draw(sf::RenderWindow& window);

private:
    enum Cell { WALL, PATH, VISITED, START, END, SOLUTION };

    struct Position {
        int x, y;
        Position(int x, int y) : x(x), y(y) {}
    };

    int width, height, cellSize;
    std::vector<std::vector<Cell>> grid;
    Position start, end;

    void generateDFS(Position pos);
    bool solveDFS(Position pos, sf::RenderWindow& window);  // Declaración de la función solveDFS
    std::vector<Position> getNeighbors(const Position& pos);
};


Maze::Maze(int width, int height, int cellSize)
    : width(width), height(height), cellSize(cellSize), start(1, 1), end(width - 2, height - 2) {
    grid.resize(width, std::vector<Cell>(height, WALL));
    generateMaze();
    grid[start.x][start.y] = START;
    grid[end.x][end.y] = END;
}

void Maze::generateMaze() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            grid[x][y] = WALL;
        }
    }
    generateDFS(start);
    grid[start.x][start.y] = START;
    grid[end.x][end.y] = END;
}

void Maze::generateDFS(Position pos) {
    grid[pos.x][pos.y] = PATH;
    std::vector<Position> directions = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };
    std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device()()));

    for (const Position& direction : directions) {
        int nx = pos.x + direction.x;
        int ny = pos.y + direction.y;

        if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && grid[nx][ny] == WALL) {
            grid[nx][ny] = PATH;
            grid[pos.x + direction.x / 2][pos.y + direction.y / 2] = PATH;
            generateDFS({ nx, ny });
        }
    }
}

bool Maze::solveMaze(sf::RenderWindow& window) {
    return solveDFS(start, window);
}
bool Maze::solveMazeBFS(sf::RenderWindow& window) {
    std::queue<Position> q;
    q.push(start);
    grid[start.x][start.y] = VISITED;

    while (!q.empty()) {
        Position pos = q.front();
        q.pop();

        if (pos.x == end.x && pos.y == end.y) {
            grid[pos.x][pos.y] = SOLUTION;
            return true;
        }

        // Visualizamos cada paso
        draw(window);
        window.display();
        sf::sleep(sf::milliseconds(200));  // Pausa para ver el progreso

        std::vector<Position> neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
        for (const Position& dir : neighbors) {
            int nx = pos.x + dir.x;
            int ny = pos.y + dir.y;

            if (nx >= 0 && ny >= 0 && nx < width && ny < height && (grid[nx][ny] == PATH || grid[nx][ny] == END)) {
                q.push({nx, ny});
                grid[nx][ny] = VISITED;  // Marcamos como visitado
            }
        }
    }
    return false;
}

bool Maze::solveDFS(Position pos, sf::RenderWindow& window) {
    if (pos.x == end.x && pos.y == end.y) {
        grid[pos.x][pos.y] = SOLUTION;
        return true;
    }

    grid[pos.x][pos.y] = VISITED;
    draw(window);
    window.display();
    sf::sleep(sf::milliseconds(200));

    std::vector<Position> neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
    for (const Position& dir : neighbors) {
        int nx = pos.x + dir.x;
        int ny = pos.y + dir.y;

        if (grid[nx][ny] == PATH || grid[nx][ny] == END) {
            if (solveDFS({ nx, ny }, window)) {
                grid[pos.x][pos.y] = SOLUTION;
                draw(window);
                window.display();
                sf::sleep(sf::milliseconds(50));
                return true;
            }
        }
    }

    grid[pos.x][pos.y] = PATH;
    return false;
}

void Maze::draw(sf::RenderWindow& window) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(x * cellSize, y * cellSize);

            switch (grid[x][y]) {
                case WALL: cell.setFillColor(sf::Color::Black); break;
                case PATH: cell.setFillColor(sf::Color::White); break;
                case VISITED: cell.setFillColor(sf::Color::Blue); break;
                case START: cell.setFillColor(sf::Color::Green); break;
                case END: cell.setFillColor(sf::Color::Red); break;
                case SOLUTION: cell.setFillColor(sf::Color::Yellow); break;
            }

            window.draw(cell);
        }
    }
}


int BFS(){
    int width = 21;
    int height = 21;
    int cellSize = 20;

    sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Es");

    Maze maze(width, height, cellSize);
    maze.generateMaze();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        maze.draw(window);
        window.display();



        if(maze.solveMazeBFS(window)){
             sf::sleep(sf::seconds(2));
             break;
        }

    }

    return 0;
}

int DFS(){
    int width = 21;
    int height = 21;
    int cellSize = 20;

    sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Es");

    Maze maze(width, height, cellSize);
    maze.generateMaze();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        maze.draw(window);
        window.display();



        if(maze.solveMaze(window)){
             sf::sleep(sf::seconds(2));
             break;
        }

    }

    return 0;
}




int menuBFSANDFS(){
    sf::RenderWindow windowBusqueda(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);


    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = windowBusqueda.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexB = 0;
    int maxItemsB = 4;
    sf::Text menuBusqueda[4];

    menuBusqueda[0].setFont(font2);
    menuBusqueda[0].setString("BFS");
    menuBusqueda[0].setCharacterSize(36);
    menuBusqueda[0].setFillColor(0 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[0].setPosition(275, 150);

    menuBusqueda[1].setFont(font2);
    menuBusqueda[1].setString("DFS");
    menuBusqueda[1].setCharacterSize(36);
    menuBusqueda[1].setFillColor(1 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[1].setPosition(275, 200);

    menuBusqueda[2].setFont(font2);
    menuBusqueda[2].setString("REGRESAR");
    menuBusqueda[2].setCharacterSize(36);
    menuBusqueda[2].setFillColor(2 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[2].setPosition(275, 250);






    while (windowBusqueda.isOpen()) {
        sf::Event event;
        while (windowBusqueda.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowBusqueda.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB - 1 + maxItemsB) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB + 1) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << menuBusqueda[indexB].getString().toAnsiString() << "\n";

                    if (indexB == 2) {

                        windowBusqueda.close();

                    }

                    if (indexB == 1) {

                        DFS();
                    }
                    if (indexB == 0) {
                        BFS();
                    }




                }
                }
                }

                windowBusqueda.clear();
        windowBusqueda.draw(SpriteFondo);
        for (int i = 0; i < maxItemsB; i++) {
            windowBusqueda.draw(menuBusqueda[i]);
        }
        windowBusqueda.display();
    }
    return 0;

}





int ESTRUCTURASNOLINEALES() {

    sf::RenderWindow windowBusqueda(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);


    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = windowBusqueda.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexB = 0;
    int maxItemsB = 4;
    sf::Text menuBusqueda[4];

    menuBusqueda[0].setFont(font2);
    menuBusqueda[0].setString("Arbol binario");
    menuBusqueda[0].setCharacterSize(36);
    menuBusqueda[0].setFillColor(0 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[0].setPosition(275, 150);

    menuBusqueda[1].setFont(font2);
    menuBusqueda[1].setString("Dijkstra");
    menuBusqueda[1].setCharacterSize(36);
    menuBusqueda[1].setFillColor(1 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[1].setPosition(275, 200);

    menuBusqueda[2].setFont(font2);
    menuBusqueda[2].setString("BFS & DFS");
    menuBusqueda[2].setCharacterSize(36);
    menuBusqueda[2].setFillColor(2 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[2].setPosition(275, 250);

    menuBusqueda[3].setFont(font2);
    menuBusqueda[3].setString("Regresar");
    menuBusqueda[3].setCharacterSize(36);
    menuBusqueda[3].setFillColor(2 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[3].setPosition(275, 300);




    while (windowBusqueda.isOpen()) {
        sf::Event event;
        while (windowBusqueda.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowBusqueda.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB - 1 + maxItemsB) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB + 1) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << menuBusqueda[indexB].getString().toAnsiString() << "\n";

                    if (indexB == 3) {

                        windowBusqueda.close();

                    }

                    if (indexB == 2) {
                        cout<<"BFS"<<endl;
                        menuBFSANDFS();
                    }
                    if (indexB == 1) {
                        cout<<"Dijkstra"<<endl;
                        dijkstra2();
                    }
                      if (indexB == 0) {
                        cout<<"arbol"<<endl;
                        drawBinaryTree();
                    }



                }
                }
                }

                windowBusqueda.clear();
        windowBusqueda.draw(SpriteFondo);
        for (int i = 0; i < maxItemsB; i++) {
            windowBusqueda.draw(menuBusqueda[i]);
        }
        windowBusqueda.display();
    }
    return 0;

}



int ESTRUCTURASLINEALES() {

    sf::RenderWindow windowBusqueda(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);


    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = windowBusqueda.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexB = 0;
    int maxItemsB = 4;
    sf::Text menuBusqueda[4];

    menuBusqueda[0].setFont(font2);
    menuBusqueda[0].setString("Stack");
    menuBusqueda[0].setCharacterSize(36);
    menuBusqueda[0].setFillColor(0 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[0].setPosition(275, 150);

    menuBusqueda[1].setFont(font2);
    menuBusqueda[1].setString("Queue");
    menuBusqueda[1].setCharacterSize(36);
    menuBusqueda[1].setFillColor(1 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[1].setPosition(275, 200);

    menuBusqueda[2].setFont(font2);
    menuBusqueda[2].setString("LinkedLists");
    menuBusqueda[2].setCharacterSize(36);
    menuBusqueda[2].setFillColor(2 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[2].setPosition(275, 250);

    menuBusqueda[3].setFont(font2);
    menuBusqueda[3].setString("Regresar");
    menuBusqueda[3].setCharacterSize(36);
    menuBusqueda[3].setFillColor(2 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[3].setPosition(275, 300);




    while (windowBusqueda.isOpen()) {
        sf::Event event;
        while (windowBusqueda.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowBusqueda.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB - 1 + maxItemsB) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB + 1) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << menuBusqueda[indexB].getString().toAnsiString() << "\n";

                    if (indexB == 3) {

                        windowBusqueda.close();

                    }

                    if (indexB == 2) {
                        cout<<indexB<<endl;
                        LinkedLists();
                    }
                    if (indexB == 1) {
                        cout<<indexB<<endl;
                        QueueAl();
                    }
                      if (indexB == 0) {
                        cout<<indexB<<endl;
                        Stacks();
                    }



                }
                }
                }

                windowBusqueda.clear();
        windowBusqueda.draw(SpriteFondo);
        for (int i = 0; i < maxItemsB; i++) {
            windowBusqueda.draw(menuBusqueda[i]);
        }
        windowBusqueda.display();
    }
    return 0;

}


int BUSQUEDA() {

    sf::RenderWindow windowBusqueda(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);


    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = windowBusqueda.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int indexB = 0;
    int maxItemsB = 3;
    sf::Text menuBusqueda[3];

    menuBusqueda[0].setFont(font2);
    menuBusqueda[0].setString("Linear Search");
    menuBusqueda[0].setCharacterSize(36);
    menuBusqueda[0].setFillColor(0 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[0].setPosition(275, 150);

    menuBusqueda[1].setFont(font2);
    menuBusqueda[1].setString("Binary Search");
    menuBusqueda[1].setCharacterSize(36);
    menuBusqueda[1].setFillColor(1 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[1].setPosition(275, 200);

    menuBusqueda[2].setFont(font2);
    menuBusqueda[2].setString("Regresar");
    menuBusqueda[2].setCharacterSize(36);
    menuBusqueda[2].setFillColor(2 == indexB ? sf::Color::Magenta : sf::Color::Black);
    menuBusqueda[2].setPosition(275, 250);




    while (windowBusqueda.isOpen()) {
        sf::Event event;
        while (windowBusqueda.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowBusqueda.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB - 1 + maxItemsB) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menuBusqueda[indexB].setFillColor(sf::Color::Black);
                    indexB = (indexB + 1) % maxItemsB;
                    menuBusqueda[indexB].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << menuBusqueda[indexB].getString().toAnsiString() << "\n";

                    if (indexB == 2) {

                        windowBusqueda.close();

                    }
                    if (indexB == 1) {
                        binarySearch();
                    }
                      if (indexB == 0) {
                        linearSearch();
                    }



                }
                }
                }

                windowBusqueda.clear();
        windowBusqueda.draw(SpriteFondo);
        for (int i = 0; i < maxItemsB; i++) {
            windowBusqueda.draw(menuBusqueda[i]);
        }
        windowBusqueda.display();
    }
    return 0;

}


int main() {

    sf::RenderWindow window(sf::VideoMode(960, 540), "Estructura de datos, algoritmos de ordenamiento y busqueda", sf::Style::Close);


    sf::Font font;
    sf::Font font2;
    sf::Texture fondo;
    if (!font.loadFromFile("src/Assets/Pacifico.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }
    if (!font2.loadFromFile("src/Assets/Raleway-Light.ttf")) {
        std::cerr << "Error loading font 2!\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!fondo.loadFromFile("src/Assets/menu.jpg")) {
        std::cerr << "Error al cargar la imagen de fondo.\n";
        return -1;
    }
    sf::Vector2u windowSize = window.getSize();
    sf::Sprite SpriteFondo(fondo);
    SpriteFondo.setScale(
        static_cast<float>(windowSize.x) / fondo.getSize().x,
        static_cast<float>(windowSize.y) / fondo.getSize().y
    );


    int index = 0;
    int maxItems = 5;
    sf::Text menu[5];

    menu[0].setFont(font2);
    menu[0].setString("Algoritmos de busqueda");
    menu[0].setCharacterSize(36);
    menu[0].setFillColor(0 == index ? sf::Color::Magenta : sf::Color::Black);
    menu[0].setPosition(275, 150);

    menu[1].setFont(font2);
    menu[1].setString("Algoritmos de ordenamiento");
    menu[1].setCharacterSize(36);
    menu[1].setFillColor(1 == index ? sf::Color::Magenta : sf::Color::Black);
    menu[1].setPosition(275, 200);

    menu[2].setFont(font2);
    menu[2].setString("Estructuras lineales");
    menu[2].setCharacterSize(36);
    menu[2].setFillColor(2 == index ? sf::Color::Magenta : sf::Color::Black);
    menu[2].setPosition(275, 250);

    menu[3].setFont(font2);
    menu[3].setString("Estructuras no lineales");
    menu[3].setCharacterSize(36);
    menu[3].setFillColor(3 == index ? sf::Color::Magenta : sf::Color::Black);
    menu[3].setPosition(275, 300);

    menu[4].setFont(font2);
    menu[4].setString("Salir");
    menu[4].setCharacterSize(36);
    menu[4].setFillColor(4 == index ? sf::Color::Magenta : sf::Color::Black);
    menu[4].setPosition(275, 350);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu[index].setFillColor(sf::Color::Black);
                    index = (index - 1 + maxItems) % maxItems;
                    menu[index].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    menu[index].setFillColor(sf::Color::Black);
                    index = (index + 1) % maxItems;
                    menu[index].setFillColor(sf::Color::Magenta);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Seleccionaste: " << menu[index].getString().toAnsiString() << "\n";
                    if (index == 4) {
                        window.close();
                    }
                    if (index == 3) {
                        ESTRUCTURASNOLINEALES();
                    }
                    if (index == 2) {
                        ESTRUCTURASLINEALES();
                    }
                    if (index == 1) {
                        ORDENAMIENTO();
                    }
                      if (index == 0) {
                        BUSQUEDA();
                    }
                }
                }
                }
             window.clear();
        window.draw(SpriteFondo);
        for (int i = 0; i < maxItems; i++) {
            window.draw(menu[i]);
        }
        window.display();
    }

    return 0;

        }
