#ifndef CARD_H
#define CARD_H
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<QtCore>
using namespace std;

const string clubs =":/images/clubs/C";//":/images/clubs/C10.jpg"
const string diamonds =":/images/diamonds/D";
const string hearts =":/images/hearts/H";
const string spades =":/images/spades/S";
const string empty =":/images/empty.jpg";
class Card{
    string buildPathPart1(char ch,int cost){
        switch (ch) {
        case 'C':
            return clubs+buildPathPart2(cost);
        case 'D':
            return diamonds+buildPathPart2(cost);
        case 'H':
            return hearts+buildPathPart2(cost);
        case 'S':
            return spades+buildPathPart2(cost);
        default:
            return empty;
        }

    }
    string buildPathPart2(int cost){
        switch (cost) {
        case 14:
            return "Ace.png";
            break;
        case 13:
            return "King.png";
            break;
        case 12:
            return "Lady.png";
        case 11:
            return "Jack.png";
        default:
            return to_string(cost)+".png";
        }

    }
public:
    //  static int static_queue;
    int id=0;
    string rang="",path="";
    int cost=0;
    Card(){}
    Card(string code){
        if(code.size()>4){
            id= stoi(code.substr(4,5));
             cost = stoi(code.substr(1,2));
            this->rang=code.at(0)+to_string(cost);
            path=buildPathPart1(code.at(0),cost);
        }
    }
    string getPath(){
        return path;
    }
    QString stringForNet(){
      //  id*rang*path*cost
        string str;
        str.append(to_string(id)+"!!!").append(rang+"!!!")
           .append(path+"!!!").append(to_string(cost));
        return QString::fromStdString(str);
    }
    void stringFromNet(QString str){
        QStringList list = str.split(QRegExp("!!!"));
      //  for(auto l : list)
               // qDebug()<<" cards string "<<l<<endl;
        if(list.size()==4){
            id=list.at(0).toInt();
            rang=list.at(1).toStdString();
            path=list.at(2).toStdString();
            cost=list.at(3).toInt();
        }else
            qDebug()<<"Error in class Cards\n";
    }
    bool operator==(const Card& rhs) const { return this->rang == rhs.rang;}
};
class CardDeck{
public:
    vector<Card*> cards={
        new Card("D14A01"),new Card("D13K02"),new Card("D12L03"),new Card("D11J04"),new Card("D10_05"),
        new Card("D09D06"),new Card("D08D07"),new Card("D07D08"),new Card("D06D09"),new Card("D05D10"),
        new Card("D04D11"),new Card("D03D12"),new Card("D02D13"),
        new Card("C14A14"),new Card("C13K15"),new Card("C12L16"),new Card("C11J17"),new Card("C10_18"),
        new Card("C09_19"),new Card("C08_20"),new Card("C07_21"),new Card("C06_22"),new Card("C05_23"),
        new Card("C04_24"),new Card("C03_25"),new Card("C02_26"),
        new Card("S14A27"),new Card("S13K28"),new Card("S12L29"),new Card("S11J30"),new Card("S10_31"),
        new Card("S09_32"),new Card("S08_33"),new Card("S07_34"),new Card("S06_35"),new Card("S05_36"),
        new Card("S04_37"),new Card("S03_38"),new Card("S02_39"),
        new Card("H14A40"),new Card("H13K41"),new Card("H12L42"),new Card("H11J43"),new Card("H10_44"),
        new Card("H09_45"),new Card("H08_46"),new Card("H07_47"),new Card("H06_48"),new Card("H05_49"),
        new Card("H04_50"),new Card("H03_51"),new Card("H02_52")
    };
    CardDeck(){


        random_device rd;
        auto rng = default_random_engine {rd()};
        shuffle(begin(cards), end(cards),rng);
    }
    void shuffle1(){
        random_device rd;
        auto rng = default_random_engine {rd()};
        shuffle(begin(cards), end(cards),rng);
    }
    ~CardDeck(){
        if(cards.size()>0)
            for(auto c :cards)
                delete c;
    }
    void show(){
        int row=0;
        for(auto c : cards){
            cout<<c->rang<<" "<<"cost "<<c->cost<<" ";row++;
            if(row==5){row=0;cout<<endl;}
        }
        cout<<endl;
    }
};
//int Card::static_queue=0;
#endif // CARD_H
