#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <cstdlib>


class Category {
    public:
        std::string name;
        std::vector<Question> questions;
    
        Category(std::string categoryName){
            name = categoryName;
        }
    
        void AddQuestion(const Question& q) {
            questions.push_back(q);
        }
};

class Question{
    public:
        std::string question;
        std::string answer1;
        std::string answer2;
        std::string answer3;
        int correctAnswer;

        Question(std::string x, std::string y, std::string z, std::string u, int v){
            question = x;
            answer1 = y;
            answer2 = z;
            answer3 = u;
            correctAnswer = v;
        }

        void Display() const{
            std::cout << "Question: " << question << "\n";
            std::cout << "Answer 1: " << answer1 << "\n";
            std::cout << "Answer 2: " << answer2 << "\n"; 
            std::cout << "Answer 3: " << answer3 << "\n";
        }

        bool CheckAnswer(int userAnswer) const {
            return userAnswer == correctAnswer;
        }
};

void ReadQuestionsFromFile(std::string filename, std::map<std::string, Category>& categories) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file!\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string category, question, ans1, ans2, ans3;
        int correctAnswer;

        std::getline(ss, category, '|');
        std::getline(ss, question, '|');
        std::getline(ss, ans1, '|');
        std::getline(ss, ans2, '|');
        std::getline(ss, ans3, '|');
        ss >> correctAnswer;

        // If category does not exist, create it
        if (categories.find(category) == categories.end()) {
            categories[category] = Category(category);
        }
        categories[category].AddQuestion(Question(question, ans1, ans2, ans3, correctAnswer));
    }

    file.close();
}

void GenerateTest(std::map<std::string, Category>& categories, int numPerCategory) {
    std::srand(time(0));

    int correctAnswers = 0;
    int wrongAnswers = 0;

    // Loop through each category
    for (auto& pair : categories) {
        std::string categoryName = pair.first;
        Category& category = pair.second;

        std::cout << "Category: " << categoryName << "\n";
        int count = 0;

        // Ask a limited number of questions
        while (count < numPerCategory && count < category.questions.size()) {
            int index = rand() % category.questions.size();  // Pick a random question
            category.questions[index].Display();  // Show question
            
            int userAnswer;
            std::cout << "Your answer: ";
            std::cin >> userAnswer;

            // Check if the answer is correct
            if (category.questions[index].CheckAnswer(userAnswer)) {
                std::cout << "Correct!\n";
                correctAnswers++;
            } else {
                std::cout << "Wrong! The correct answer was " << category.questions[index].correctAnswer << "\n";
                wrongAnswers++;
            }

            count++;  // Move to the next question
            std::cout << "----------------\n";
        }
    }

    std::cout << "Correct Answers: " << correctAnswers << "\n";
    std::cout << "Wrong Answers: " << wrongAnswers << "\n";

    if (wrongAnswers >= 2){
        std::cout << "You failed the test\n";
    }
    
}



int main() {
    std::map<std::string, Category> categories;
    
    // Read questions from file
    ReadQuestionsFromFile("questions.txt", categories);

    // Start the quiz
    int numQuestionsPerCategory = 2;
    GenerateTest(categories, numQuestionsPerCategory);

    return 0;
}

 