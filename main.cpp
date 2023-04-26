#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
using namespace std;


void encryptWordsToFile(const vector<string>& words, const string& filename)
{
    ofstream file(filename, ios::app); 

    for (const string& word : words) 
    {
        string encryptedWord = word; 

        for (char& c : encryptedWord) 
        {
            if (isalpha(c)) 
            {
                if (islower(c)) 
                {
                    c = (c - 'a' + 1) % 26 + 'a'; 
                }
                else 
                {
                    c = (c - 'A' + 1) % 26 + 'A'; 
                }
            }
        }

        file << encryptedWord << " "; 
    }

    file.close(); 
}



void decryptWordsFromFile(const string& filename, vector<string>& words)
{
    ifstream file(filename); 

    if (file.is_open())
    {
        string word;

        while (file >> word) 
        {
            for (char& c : word) 
            {
                if (isalpha(c)) 
                {
                    if (islower(c)) 
                    {
                        c = (c - 'a' + 25) % 26 + 'a'; 
                    }
                    else 
                    {
                        c = (c - 'A' + 25) % 26 + 'A'; 
                    }
                }
            }

            words.push_back(word); 
        }

        file.close(); 
    }
}




int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    const int MAX_WRONG = 8; 
    vector<string> words; 
    vector<string> descwords; 
    words.push_back("guess");
    words.push_back("hangman");
    words.push_back("difficult");
    encryptWordsToFile(words, "slova.txt");
    srand(static_cast<unsigned int>(time(0)));
    decryptWordsFromFile("slova.txt", descwords);
    random_shuffle(descwords.begin(), descwords.end());
    const string THE_WORD = descwords[0]; 
    int wrong = 0; 
    std::string soFar(THE_WORD.size(), '-'); 
    std::string used = ""; 

    std::cout << "Welcome to Hangman. Good luck!\n";

    sf::RenderWindow window(sf::VideoMode(400, 400), "Hangman");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text usedLetters("", font, 20);
    usedLetters.setPosition(10, 10);

    sf::Text wordSoFar("", font, 40);
    wordSoFar.setPosition(50, 300);

    sf::RectangleShape hangmanBase(sf::Vector2f(100, 10));
    hangmanBase.setPosition(50, 200);
    hangmanBase.setFillColor(sf::Color::White);

    sf::RectangleShape hangmanPole(sf::Vector2f(10, 150));
    hangmanPole.setPosition(100, 50);
    hangmanPole.setFillColor(sf::Color::White);

    sf::RectangleShape hangmanTop(sf::Vector2f(50, 10));
    hangmanTop.setPosition(100, 50);
    hangmanTop.setFillColor(sf::Color::White);

    sf::RectangleShape hangmanRope(sf::Vector2f(10, 25));
    hangmanRope.setPosition(140, 60);
    hangmanRope.setFillColor(sf::Color::White);

    sf::CircleShape hangmanHead(20);
    hangmanHead.setPosition(125, 85);
    hangmanHead.setFillColor(sf::Color::White);

    sf::RectangleShape hangmanBody(sf::Vector2f(10, 100));
    hangmanBody.setPosition(135, 105);
    hangmanBody.setFillColor(sf::Color::White);

    sf::RectangleShape hangmanLeftArm(sf::Vector2f(50, 10));
    hangmanLeftArm.setPosition(100, 140);
    hangmanLeftArm.setFillColor(sf::Color::White);
    hangmanLeftArm.setRotation(45);

    sf::RectangleShape hangmanRightArm(sf::Vector2f(50, 10));
    hangmanRightArm.setPosition(170, 140);
    hangmanRightArm.setFillColor(sf::Color::White);
    hangmanRightArm.setRotation(-45);

    sf::RectangleShape hangmanLeft(sf::Vector2f(50, 10));
    hangmanLeft.setPosition(110, 205);
    hangmanLeft.setFillColor(sf::Color::White);
    hangmanLeft.setRotation(-45);
    sf::RectangleShape hangmanRight(sf::Vector2f(50, 10));
    hangmanRight.setPosition(160, 205);
    hangmanRight.setFillColor(sf::Color::White);
    hangmanRight.setRotation(45);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode >= 'a' && event.text.unicode <= 'z')
                {
                    char letter = static_cast<char>(event.text.unicode);
                    if (used.find(letter) == std::string::npos)
                    {
                        used += letter;
                        if (THE_WORD.find(letter) != std::string::npos)
                        {
                            for (int i = 0; i < THE_WORD.size(); ++i)
                            {
                                if (THE_WORD[i] == letter)
                                {
                                    soFar[i] = letter;
                                }
                            }
                        }
                        else
                        {
                            ++wrong;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        
        switch (wrong)
        {
        case 8:
            window.draw(hangmanRight);
        case 7:
            window.draw(hangmanLeft);
        case 6:
            window.draw(hangmanRightArm);
        case 5:
            window.draw(hangmanLeftArm);
        case 4:
            window.draw(hangmanBody);
        case 3:
            window.draw(hangmanHead);
        case 2:
            window.draw(hangmanRope);
        case 1:
            window.draw(hangmanTop);
        case 0:
            window.draw(hangmanPole);
            window.draw(hangmanBase);
            break;
        }

        
        usedLetters.setString("Used Letters: " + used);
        window.draw(usedLetters);

        
        wordSoFar.setString(soFar);
        window.draw(wordSoFar);

        
        if (soFar == THE_WORD)
        {
            sf::Text winText("You won!", font, 50);
            winText.setPosition(50, 150);
            window.draw(winText);
            window.display();
            sf::sleep(sf::seconds(2));
            window.close();
        }

        if (wrong == MAX_WRONG)
        {
            sf::Text loseText("You lost! The word was " + THE_WORD, font, 50);
            loseText.setPosition(50, 150);
            window.draw(loseText);
            window.display();
            sf::sleep(sf::seconds(2));
            window.close();
        }

        window.display();
    }
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end - start;
    double seconds = elapsed_time.count();

    cout << "Program execution time: " << seconds << " seconds\n";
    cout <<"Wrong attempts: " << wrong << endl;
    cout << "The word: " << THE_WORD << endl;
    cout << "Used words: " << used;
    return 0;
}






















