#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>


class WordCounter {
private:
    std::list<std::string> lines;
    std::map<std::string, int> wordFrequency;
    size_t totalWords;

    // Функция для разделения строки на слова
    std::list<std::string> tokenize(const std::string& line) const {
        std::list<std::string> words;
        std::string word;

        for (char ch : line) {
            if (std::isalnum(ch)) {
                word += std::tolower(ch);
            } else if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }

        if (!word.empty()) {
            words.push_back(word);
        }

        return words;
    }

public:
    WordCounter() : totalWords(0) {}

    // Чтение строк из файла
    bool readFile(const std::string& inputFileName) {
        std::ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            std::cerr << "Ошибка! Не удалось открыть входной файл." << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }

        inputFile.close();
        return true;
    }

    // Подсчёт частоты слов
    void countWords() {
        for (const auto& textLine : lines) {
            std::list<std::string> words = tokenize(textLine);
            for (const auto& word : words) {
                wordFrequency[word]++;
                totalWords++;
            }
        }
    }

    // Запись результатов в CSV файл
    bool writeCSV(const std::string& outputFileName) const {
        std::vector<std::pair<std::string, int>> sortedWords(wordFrequency.begin(), wordFrequency.end());

        std::sort(sortedWords.begin(), sortedWords.end(), [](const auto& a, const auto& b) {
            return b.second < a.second; // Сортировка по убыванию частоты
        });

        std::ofstream outputFile(outputFileName);
        if (!outputFile.is_open()) {
            std::cerr << "Ошибка! Не удалось открыть выходной файл." << std::endl;
            return false;
        }

        outputFile << "Слово,Частота,Повторение (%)" << std::endl;

        for (const auto& [word, count] : sortedWords) {
            double frequencyPercent = (static_cast<double>(count) / totalWords) * 100.0;
            outputFile << word << "," << count << "," << std::fixed << std::setprecision(2) << frequencyPercent << std::endl;
        }

        outputFile.close();
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " input.txt output.csv" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    WordCounter wordCounter;

    if (!wordCounter.readFile(inputFileName)) {
        return 1;
    }

    wordCounter.countWords();

    if (!wordCounter.writeCSV(outputFileName)) {
        return 1;
    }

    std::cout << "CSV-файл успешно создан: " << outputFileName << std::endl;
    return 0;
}