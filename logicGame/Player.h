#ifndef PLAYER_H
#define PLAYER_H
#include"Card.h"
#include<QtCore>
class Player{
    int id=0;
    string name="";
    vector <Card *> cards;
    int statusWinner = 0; // поиск среди победителей с max статусом
    bool winner=false;
    bool diller=false;
    bool fold =false;
    // int MaxStavka = 0;
    string combo="";
    pair<int,int> comb_and_last=make_pair(0,0); // карта комбинации , карта(сумма) вне комбинации
    int money;
    bool isComputer = false;
    int userDescriptor = 0;
public:
    Player(string name,int id,int m){
        this->name=name;
        this->id=id;
        money=m;
    }
    Player(){}

    void setUserDescriptor(int i){
        userDescriptor =i;
    }
    int getUserDescriptor(){
        return userDescriptor;
    }
    void setIsComputer(bool b){
        isComputer=b;
    }
    bool getIsComputer(){
        return isComputer;
    }
    void setCards(Card * card){
        cards.push_back(card);
    }
    void clearCard(){
        cards.clear();
        winner=false;
        diller=false;
        if(money>0)
            fold =false;
    }
    vector<Card *>& getCards(){
        return cards;
    }
    string getName(){
        return name;
    }
    int getId(){
        return  id;
    }
    void setId(int n){
        id=n;
    }
    //    void setMaxStavka(int s){
    //        MaxStavka =s;
    //    }
    string show(){
        string str=name;
        if(winner) str =" Winner ";
        else str =" Loser ";
        str.append(combo+" ")
                .append("("+to_string(comb_and_last.first)+","
                        +to_string(comb_and_last.second)+")");

        //.append(comb_and_last.first<<" допкарта "<<comb_and_last.second<<endl;
        return str;
    }
    int raise(int r){

        if(r<=money){
            money -=r;
            return r;// здесь для экономии просто переназначает ставку
        }else if( money >0){
            r=money;
            money=0;
            return r;
        }else{
            // FOLD
            return decideStep(3,r);
        }
    }
    int decideStep(int pushButton,int stavke){
        // fold , raise, call;
        //  show();
        if(fold)
            return 0;
        else{
            //  if(diller)cout<<name<<" очередь ";
            //  cout<<name<<" THINK!!"<<endl;

            //               if(MaxStavka==stavke)                       // p префлоп t терн
            //                cout<<"Raise 1, fold 3, check 4"<<endl;
            //               else
            //                   cout<<"Raise 1, call 2, fold 3"<<endl;

            switch (pushButton) {
            case 1:
                return raise(stavke);
            case 2:
                //  cout<<" call "<<stavke<<endl;
                money -=stavke;
                //  MaxStavka=stavke;
                return stavke;
            case 3:
                fold=1;
                combo="Fold";
                //    cout<<" fold "<<endl;
                return 0;
            case 4:
                //     cout<<" check "<<endl;
                return 0;
            }
            return 0;
        }

    }
    int getBlind(int b){
        if(!fold){
            //   cout<<name<<" сделал ставку "<<b<<endl;
            if(money<=0) {
                fold=1;
                combo="No money";
                return 0;
            }
            //        else if( b <money){
            //            b=money;
            //            money =0;
            //            return b;
            //        }
            else{
                money -=b;
                return b;
            }
        }
        else
            //    cout<<name<<" я пустой или фолд "<<b<<endl;

            return 0;
    }
    void addCash(int cash){
        money +=cash;
    }
    int getMoney(){
        return money;
    }
    void setMoney(int n){
        money=n;
    }
    //    int All_in(){
    //        int m = money;
    //        money = 0;
    //        return m;
    //    }
    void setDiller(bool d){diller=d;}
    bool getDiller(){return diller;}
    void setFold(bool d){
        combo="Fold";
        fold=d;}
    bool getFold(){return fold;}
    void setWinner(bool d){winner=d;}
    bool getWinner(){return winner;}
    void setCombo(string str){combo=str;}
    string getCombo(){return combo;}
    void setCombLast(pair<int,int> p){
        comb_and_last = p;
    }
    pair<int,int>  getCombLast(){
        return   comb_and_last;
    }
    void setStatusWinner(int s){statusWinner=s;}
    int getStatusWinner(){return statusWinner;}

    QString getInfoPlayer(){
        // id/name/money
        QString info="";
        QString spt="###";
        info.append(QString::number(id)+spt).
                append(QString::fromStdString(name)+spt).
                append(QString::number(money)+spt);
        if(cards.size()==2)
            for(auto c:cards)
                info.append(c->stringForNet()+spt);
        return info;
    }
    void setInfoPlaeyr(QString info){
        // id/name/money
        QString spt= "###";
        QRegExp exp(spt);
        QStringList list =  info.split(exp);
        list.pop_back();
        // qDebug()<<"setInfoPlaeyr "<<info<<endl;
        //  for(auto l :list)qDebug()<<l<<" **** ";qDebug()<<"list size "<<list.size()<<"\n";

        if(list.size()>0)
            id= list.at(0).toInt();
        if(list.size()>1)
            name = list.at(1).toStdString();
        if(list.size()>2)
            money = list.at(2).toInt();
        if(list.size()>4){
            cards.clear();
            Card *c = new Card();
            c->stringFromNet(list.at(3));
            cards.push_back(c);
            c = new Card();
            c->stringFromNet(list.at(4));
            cards.push_back(c);

        }
    }

};

#endif // PLAYER_H
