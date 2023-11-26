#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
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

    // 예금 계산 메서드
    double calculateTotalAmount() const {
        double totalAmount = principal; // 최종 지불액 초기화

        for (int month = 1; month <= maturityPeriod; ++month) {
            totalAmount += monthlyDeposit; // 월 납입액 추가
            totalAmount *= (1 + interestRate / 12); // 월 이자 계산
        }

        return totalAmount;
    }

    // 결과 출력 메서드
    void displayResult() const {
        cout << "\n정기예금 계좌 개설이 완료되었습니다.\n";
        cout << "이름: " << name << "\t생년월일: " << birthdate << endl;
        cout << "최종 지불액: " << fixed << setprecision(2) << calculateTotalAmount() << " 원\n";
    }
    
    // 계좌 정보를 파일에 저장
    void saveToFile() const {
        ofstream outFile("deposit_info.txt", ios::app);
        if (outFile.is_open()) {
            outFile << name << " " << birthdate << " " << principal << " " << interestRate << " " << maturityPeriod << " " << monthlyDeposit << endl;
            outFile.close();
        } else {
            cout << "파일을 열 수 없습니다." << endl;
        }
    }

    // 파일에서 계좌 정보를 불러오기
    static vector<DepositProduct> loadFromFile() {
        vector<DepositProduct> accounts;
        ifstream inFile("deposit_info.txt");
        if (inFile.is_open()) {
            while (true) {
                string n, bd;
                double p, rate, deposit;
                int period;
                if (!(inFile >> n >> bd >> p >> rate >> period >> deposit)) {
                    break;  // 파일 끝에 도달하면 종료
                }
                DepositProduct account(n, bd, p, rate, period, deposit);
                accounts.push_back(account);
            }
            inFile.close();
        } else {
            cout << "파일을 열 수 없습니다." << endl;
        }
        return accounts;
    }
};

class FinancialInformation {
public:
    // todo2 : 금융 정보에 관련된 속성과 기능을 정의
};


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
    // todo6 : 금융 상품 정보 기능을 구현
}

int main() {
    int mainMenuChoice;
    
    do {
        cout << "-----------------------------------예금 상품 제작-------------------------------" << endl;
        cout << "1. 예금 상품 만들기 \t 2. 본인 상품 확인하기  \t 3. 추천 상품 확인하기  \t 4. 종료"<< endl;
        cout << "----------------------------------------------------------------------------" << endl;
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
                financialInformation();
                break;
            case 4:
                cout << "프로그램을 종료합니다." << endl;
                break;
            default:
                cout << "올바르지 않은 선택입니다. 다시 선택하세요." << endl;
                break;
        }
    } while (mainMenuChoice != 4);

    return 0;
}
  
