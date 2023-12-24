#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <exception>
#include <chrono>
#include <algorithm>
#include <cmath>
using namespace std;

class DepositProduct {
public:
    string name;            // 사용자 이름
    string birthdate;       // 사용자 생년월일
    double principal;       // 원금
    double interestRate;    // 이자율
    int maturityPeriod;     // 만기 기간 (월 단위)
    double monthlyDeposit;  // 월 납입액

public:
    // 생성자
    DepositProduct(const string& n, const string& bd, double p, double rate, int period, double deposit)
        : name(n), birthdate(bd), principal(p), interestRate(rate / 100.0), maturityPeriod(period), monthlyDeposit(deposit) {}

    double calculateTotalAmount() const {
        double totalAmount = principal;

        for (int month = 1; month <= maturityPeriod; ++month) {
            totalAmount += monthlyDeposit;

            double monthlyInterest = totalAmount * interestRate / 12;
            totalAmount += monthlyInterest;
        }

        return totalAmount;
    }

    void displayResult() const {
        cout << "\n정기예금 계좌 개설이 완료되었습니다.\n";
        cout << "이름: " << name << " \t생년월일: " << birthdate << std::endl;
        cout << "최종 지불액: " << std::fixed << std::setprecision(2) << calculateTotalAmount() << " 원\n";
    }

    void saveToFile() const {
        ofstream outFile("deposit_info.txt", std::ios::app);
        if (outFile.is_open()) {
            outFile << name << " " << birthdate << " " << principal << " " << interestRate << " " << maturityPeriod << " " << monthlyDeposit << std::endl;
            outFile.close();
        } else {
            cout << "파일을 열 수 없습니다." << std::endl;
        }
    }

    static vector<DepositProduct> createdProducts;

    static const DepositProduct& getLastCreatedProduct() {
        if (!createdProducts.empty()) {
            return createdProducts.back();
        } else {
            throw logic_error("No products created yet.");
        }
    }

    static vector<DepositProduct> loadFromFile() {
        createdProducts.clear();

        vector<DepositProduct> accounts;
        ifstream inFile("deposit_info.txt");
        if (inFile.is_open()) {
            while (true) {
                string n, bd;
                double p, rate, deposit;
                int period;
                if (!(inFile >> n >> bd >> p >> rate >> period >> deposit)) {
                    break;
                }
                DepositProduct account(n, bd, p, rate, period, deposit);
                accounts.push_back(account);
            }
            inFile.close();
        } else {
            cout << "파일을 열 수 없습니다." << std::endl;
        }

        createdProducts = accounts;  // Store loaded data in createdProducts

        return accounts;
    }
};

class BankProduct {
public:
    string bankName;
    string productName;
    double maxInterestRate;
    double basicInterestRate;
    
    BankProduct(const string& bn, const string& pn, double maxRate, double basicRate)
            : bankName(bn), productName(pn), maxInterestRate(maxRate), basicInterestRate(basicRate){}
};

vector<BankProduct> readBankProductsFromFile(){
    vector<BankProduct> bankProducts;
    
    ifstream inFile("bank_product.txt");
    if(inFile.is_open()){
        while(true){
            string bankName, productName;
            double maxInterestRate, basicInterestRate;
            
            if(!(inFile >> bankName >> productName >> maxInterestRate >> basicInterestRate)){
                break;
            }
            
            BankProduct product(bankName, productName, maxInterestRate, basicInterestRate);
            bankProducts.push_back(product);
        }
        inFile.close();
    }else {
        cout << "파일을 열 수 없습니다." << endl;
    }
    return bankProducts;
}

vector<DepositProduct> DepositProduct::createdProducts;

void recommendSimilarProducts(const DepositProduct& userProduct, const vector<BankProduct>& bankProducts) {
    cout << "\n유사한 은행 상품 추천:\n";

    vector<BankProduct> similarProducts;
    for (const auto& bankProduct : bankProducts) {
        if (abs(userProduct.interestRate - bankProduct.maxInterestRate) < 0.5 &&
            userProduct.maturityPeriod == 12) {  // 예시로 만기 기간을 12로 설정
            similarProducts.push_back(bankProduct);
        }
    }

    sort(similarProducts.begin(), similarProducts.end(),
              [](const BankProduct& a, const BankProduct& b) {
                  return a.maxInterestRate > b.maxInterestRate;
              });

    const int numRecommendations = 3;
    for (int i = 0; i < min(numRecommendations, static_cast<int>(similarProducts.size())); ++i) {
        cout << similarProducts[i].bankName << "/" << similarProducts[i].productName
                  << "/최고 " << similarProducts[i].maxInterestRate << "%/기본 " << similarProducts[i].basicInterestRate << "%" << endl;
    }
}

void createDepositProduct() {
    // 사용자로부터 입력 받기
    string name, birthdate;
    double principal, interestRate, monthlyDeposit;
    int maturityPeriod;

    cout << "이름을 입력하세요: ";
    cin >> name;

    cout << "생년월일을 입력하세요 (YYYYMMDD 형식): ";
    cin >> birthdate;

    cout << "원금을 입력하세요: ";
    cin >> principal;

    cout << "이자율을 입력하세요 (%): ";
    cin >> interestRate;

    cout << "만기 기간을 입력하세요 (월 단위): ";
    cin >> maturityPeriod;

    cout << "월 납입액을 입력하세요: ";
    cin >> monthlyDeposit;

    // DepositProduct 클래스를 사용하여 예금 계좌 개설 및 결과 출력
    DepositProduct depositAccount(name, birthdate, principal, interestRate, maturityPeriod, monthlyDeposit);
    depositAccount.displayResult();
    depositAccount.saveToFile();  // 파일에 정보 저장
}

void checkOwnProduct() {
    // 파일에서 계좌 정보 불러오기
    vector<DepositProduct> accounts = DepositProduct::loadFromFile();

    if (accounts.empty()) {
        cout << "아직 개설된 계좌가 없습니다." << endl;
    } else {
        // 사용자로부터 이름과 생년월일 입력 받기
        string name, birthdate;
        cout << "이름을 입력하세요: ";
        cin >> name;
        
        cout << "생년월일을 입력하세요 (YYYYMMDD 형식): ";
        cin >> birthdate;

        // 해당 사용자의 계좌 찾아서 출력
        bool found = false;
        for (const auto& account : accounts) {
            if (account.name == name && account.birthdate == birthdate) {
                account.displayResult();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "해당하는 사용자의 계좌가 없습니다." << endl;
        }
    }
}

void financialInformation() {
    try{
        ifstream bankfile("bank_product.txt");
        if(!bankfile)
            throw runtime_error("파일을 여는데 실패했습니다.");
        string line;
        while(getline(bankfile, line)){
            cout << line << endl;
        }
        bankfile.close();
    } catch(exception &e){
        cerr << "에러 : " << e.what() << endl;
    }
}

void displayExitMessage() {
    cout << "프로그램을 종료합니다." << std::endl;
}

int main() {
    int mainMenuChoice;
    
    do {
        cout << "-----------------------------------<예금 상품 제작 프로그램>-------------------------------" << endl;
        cout << "1. 예금 상품 만들기 \t 2. 본인 상품 확인하기  \t 3. 추천 상품 확인하기  \t 4. 종료"<< endl;
        cout << "-------------------------------------------------------------------------------------" << endl;
        cout << "하시고자 하는 업무를 선택하여 주세요 : ";
        cin >> mainMenuChoice;

        switch (mainMenuChoice) {
            case 1:
                createDepositProduct();
                break;
            case 2:
                checkOwnProduct();
                break;
            case 3:
                /*
                // 사용자가 만든 상품 정보 로드
                DepositProduct::loadFromFile();

                // 은행 상품 정보 로드
                vector<BankProduct> bankProducts = readBankProductsFromFile("bank_product.txt");

                if (!bankProducts.empty() && !DepositProduct::createdProducts.empty()) {
                    const DepositProduct& userProduct = DepositProduct::getLastCreatedProduct();
                    recommendSimilarProducts(userProduct, bankProducts);
                } else {
                    cout << "은행 상품 정보 또는 사용자 상품 정보가 없습니다." << endl;
                }
                 */
                financialInformation();
                break;
            
            case 4:
                displayExitMessage();
                break;
                
            default:
                cout << "올바르지 않은 선택입니다. 다시 선택하세요." << std::endl;
                break;
        }
    } while (mainMenuChoice != 4);

    return 0;
}
  
