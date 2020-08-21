#ifndef GAME_H
#define GAME_H
#include"Player.h"
#include<queue>
#include<time.h>
#include"FindWinner.h"
#include<QEventLoop>
#include<QThread>
#include<QString>
#include<QStringList>
class Game: public QObject
{
    Q_OBJECT
signals:
    void signalProtocolTrade(string str,string color);
    void protocolTradeBlack(string);
    void protocolTradeGreen(string);
    void protocolTradeRed(string);
    void signalAllClient(QString,QString);
    void signalSendTradeToUser(int,int);
public:
    Game(){}
    Game(deque<Player *> players, int blind ):stack(new CardDeck()){//  перемешанная калода
        srand(time(NULL));
        this->blind=blind;
        this->player=players;
        player[rand()% (int)player.size()]->setDiller(1); // диллер случайно
        // player.at(0)->setDiller(1);
        //  makeDillerFirst();
        lastRichMan=player.size(); // все к гого есть деньги

    }
    Game(int players,int money, int blind):stack(new CardDeck()){//  перемешанная калода
        srand(time(NULL));
        this->blind=blind;
        player.push_back(new Player("Human player",1,money)); // first diller
        // player.at(0)->setDiller(1);

        if(players>1 && players <11){
            for(int i  = 1; i<players;i++){
                string name = string("Comp player");
                Player * pl = new Player(name.append(to_string(i)),i+1,money);
                pl->setIsComputer(true);
                player.push_back(pl);
            }
        }
        player[rand()% (int)player.size()]->setDiller(1); // диллер случайно
        // player.at(2)->setDiller(1);
        //  makeDillerFirst();
        lastRichMan=player.size(); // все к гого есть деньги

    }

    void gameToString(string code){
        //  bank=:stavka:blind:;  0:1:2:3
        QString str,spt="-";

        str.append(QString::number(bank)+spt).append(QString::number(stavka)+spt)
                .append(QString::number(blind)+spt);
        str.append("-");
        //Players
        spt="@";
        for(auto p : player)
            str.append(p->getInfoPlayer()+spt);
     //   str.append("\n");
        emit  signalAllClient( str,QString::fromStdString(code));

    }
    void commonCardsToString(){
        QString str,spt="@";
        for(auto c :commonCards)
            str.append(c->stringForNet()+spt);
       // str.append("\n");
        qDebug()<<" On server common cards"<<str<<endl;
        emit  signalAllClient( str,"3");
    }
    void stringToGame(QString info){
        //  qDebug()<<"stringToGame "<<info<<endl;
        QStringList list =  info.split(QRegExp("-"));
        list.removeAll("");
       // for(auto l : list )
       //     qDebug()<<"stringToGame "<<l<<endl;
        if(list.size()==4){ // без общих карт
            //  bank=:stavka:blind:;  0:1:2:3

            QStringList listPlayers =  list.at(3).split(QRegExp("@"));
            listPlayers.removeAll("");

            player.clear();
            for(auto l : listPlayers ){
                //  qDebug()<<"listPlayers "<<l<<endl;
                Player * p = new Player();
                p->setInfoPlaeyr(l);
                p->show();
                player.push_back(p);
            }
        }

    }

    ~Game(){
        if(player.size()>0){
            for(auto p :player)
                delete p;
        }
        if(commonCards.size()>0){
            for(auto c :commonCards)
                delete c;
        }
        delete stack;
    }

    vector<Card *>& getCommomCards(){
        return commonCards;
    }
    deque<Player *>& getPlayers(){
        return player;
    }

    void  preflop(){
        // блайнды до раздачи
        makeDillerFirst();
        protocol.append(player.at(0)->getName()+" is Diller");
        protocolTradeGreen(protocol);
        emit signalProtocolTrade(protocol,"G");protocol.clear();
        if(player.size()==2){
            stavka =  player.at(0)->getBlind(blind);// задает игрок
            notEnoughMoney(stavka);
            smallBlind();
            protocol.append(player.at(0)->getName())
                    .append(" post Small blind (")
                    .append(to_string(stavka)).append(")$\n");
            newStep();
            stavka = bigBlind();
            notEnoughMoney(stavka);
            player.at(0)->getBlind(stavka);
            protocol.append(player.at(0)->getName())
                    .append(" post Big blind (")
                    .append(to_string(stavka)).append(")$");
        }else{
            newStep();
            stavka = player.at(0)->getBlind(blind);
            notEnoughMoney(stavka);
            smallBlind();
            protocol.append(player.at(0)->getName())
                    .append(" post Small blind (")
                    .append(to_string(stavka)).append(")$\n");
            newStep();
            stavka = bigBlind();
            notEnoughMoney(stavka);
            player.at(0)->getBlind(stavka);
            protocol.append(player.at(0)->getName())
                    .append(" post Big blind (")
                    .append(to_string(stavka)).append(")$");
        }
        protocolTradeBlack(protocol);
        emit signalProtocolTrade(protocol,"B");
        protocol.clear();

        //player.at(0)->setMaxStavka(stavka); // на префлопе максимальная у Бигблейда

        for(int i = 0; i<2;i++) // раздаем по 2 карты игрокам
            for(auto p :player){
                if(p->getFold()==false){
                    p->setCards(stack->cards.back());
                    stack->cards.pop_back();
                }

            }


        gameToString("2");

    }
    void flop(){
        protocol = " FLOP ";

        for(int i = 0;i<3;i++){
            commonCards.push_back(stack->cards.back());
            stack->cards.pop_back();
            protocol.append( buildHightNameCard( commonCards[i]->rang,commonCards[i]->cost)+" ");
        }
        commonCardsToString();
        protocolTradeRed(protocol);
      //  emit signalProtocolTrade(protocol,"R");
        protocol.clear();
        newStep();// дилер ходит последним

        //   trade();
    }
    void turn(){
        protocol=" TURN ";
        commonCards.push_back(stack->cards.back());
        stack->cards.pop_back();
        for(auto c : commonCards)
            protocol.append( buildHightNameCard( c->rang,c->cost)+" ");

         commonCardsToString();
        protocolTradeRed(protocol);
      //  emit signalProtocolTrade(protocol,"R");
        protocol.clear();

        // trade(); // на один ход меньше так как после диллера ????
    }
    void river(){
        protocol=" RIVER ";
        commonCards.push_back(stack->cards.back());
        stack->cards.pop_back();
        for(auto c : commonCards)
            protocol.append( buildHightNameCard( c->rang,c->cost)+" ");
        commonCardsToString();

        protocolTradeRed(protocol);
      //  emit signalProtocolTrade(protocol,"R");
        protocol.clear();

    }
    void howWinner(){
        for(auto p :player){
                protocol.append(p->getName()+ " ")
                        .append(buildHightNameCard(p->getCards()[0]->rang,
                                p->getCards()[0]->cost))
                        .append(" ")
                        .append(buildHightNameCard(p->getCards()[1]->rang,
                                p->getCards()[1]->cost))
                        .append("\n");


        }
        emit signalProtocolTrade(protocol,"B");
        protocolTradeBlack(protocol);
        protocol.clear();
        fw.findWinner(commonCards,player);
        giveMoneyWinner();

      //  gameToString("2");
        for(auto p : player){
            protocol.append(p->getName()+" is ")
                    .append(p->show());
            protocolTradeGreen(protocol);
            emit signalProtocolTrade(protocol,"G");
            protocol.clear();
        }
        // cout<<endl;
    }

    void show(){
        cout<<"Common cards ";
        for(auto c: commonCards)
            cout<<c->rang<<" ";
        cout<< "\nBank ="<<bank<<endl;
        for(auto p :player)
            p->show();
    }

    //Префлоп, Флоп, Торн, Ривер.
    bool getEndGame(){
        return  endGame;
    }
    void setEndGame(bool b){
        endGame = b;
    }
    void trade(){

        if(player.size()>0){
            for(int st =0;st<(int)player.size();){
                st+= newStep();
                if(player[0]->getFold())continue;
                else{

                    if(player[0]->getFold()==false){

                        int newStavke=0;
                        protocol.append( player[0]->getName()+" ");
                        if(player[0]->getIsComputer()){
                            pair<int,int>comp= ifPlayerComputer(player[0]);
                            newStavke=  player[0]->decideStep(comp.first,comp.second);
                            protocol.append(getNameOfChoose(comp.first));

                        }else{

                            protocolTradeGreen(player[0]->getName()+"....");
                          //  emit signalProtocolTrade(player[0]->getName()+"....","G");
                            //TO DO узнать дескриптор сокета
                            // отправить запрос и получить кнопку и ставку
                            if(player[0]->getUserDescriptor()>0)
                                   setPlayerChooseForNet(player[0]->getUserDescriptor());
                            // кнопка, ставка

                            _loop.exec(); //-- Ждём, пока будет вызван exit();

                            newStavke=  player[0]->decideStep(playerChoose.first,playerChoose.second);

                            protocol.append(getNameOfChoose(playerChoose.first));
                        }
                        bank +=newStavke;


                        if(newStavke>0)protocol.append(to_string(newStavke)+"$");
                        protocolTradeBlack(protocol);
                      emit signalProtocolTrade(protocol,"B");

                        protocol.clear();


                        if(newStavke>stavka){
                            // player[0]->setMaxStavka(newStavke);
                            stavka=newStavke;
                            //   trade();
                        }
                        checkFold();// если все сбросили то выигрывает последний
                        if(endGame)
                            return;

                    }

                    /////
                }
            }

        }

    }
    void nextPlayer(){
        _loop.exit();
    }
    int getStavke(){
        return stavka;
    }
    void setStavka(int s){
        stavka=s;
    }
    void setPlayerChoose(int button,int stvk){
        playerChoose=make_pair(button,stvk);
    }
    void setPlayerChooseForNet(int descriptor){
        //  playerChoose=make_pair(button,stvk);

        qDebug()<<"USer decriptor "<<descriptor;
        QString str;
        str.append(QString::number(descriptor)).append("@")
            .append(QString::number(stavka));      
          emit signalSendTradeToUser(descriptor,stavka);



    }
public slots:
    void slotAnswerFromCLient(QString answer){
         qDebug()<<"slotAnswerForCLient(QString answer) "<<answer;
         QStringList list = answer.split("@");
         setPlayerChoose(list.at(0).toInt(),list.at(1).toInt());
      nextPlayer();
    }

public:
    void checkMoney(){
        lastRichMan=player.size();
        for (auto p : player)
            if(p->getMoney()<=0)
                lastRichMan--;
        if(lastRichMan==1)
            endGame=true;
        else
            endGame=false;
    }
    void newStack(){
        stack = new CardDeck;
    }
    void setDiller(){
        player[rand()% (int)player.size()]->setDiller(1); // диллер случайно
    }
    int getBank(){
        return  bank;
    }
    void addToBank(int n){
        bank +=n;
    }
private:
    int smallBlind(){
        bank +=stavka;
        return  stavka;

    }
    int bigBlind(){
        stavka *=2;
        bank +=stavka;

        return  stavka;
    }
    void checkFold(){
        int counter = player.size();

        for(auto p :player){
            if(p->getFold()==true)counter--;
            //  cout<<" Check folder"<<counter<<endl;
        }

        if(counter==1){
            for(auto p :player)
                if(p->getFold()==false){
                    //     cout<<" Check folder 2 "<<counter<<endl;
                    p->setWinner(1);
                    p->setCombo("You 1 not fold");
                    endGame=true;
                }
        }
          // gameToString("2");
    }
    int newStep(){
        if(player.size()>0){
            auto first = player.front(); // переход очереди
            if(first->getMoney()<blind){
                // first->setMoney(0);
                first->setFold(true);
            }
            player.pop_front();
            player.push_back(first);
            //    cout<<" New STEP  "<<player[0]->getName()<<" -> "<<first->getName()<<endl;

            if(player[0]->getFold())
                return 1+ newStep();
        }
        return 1;
    }
    void makeDillerFirst(){
        Player * p ;
        while (player[0]->getDiller() == false) { // устанавливаем диллера на первое место
            p = player.front();
            player.pop_front();
            player.push_back(p);
        }


    }
    void giveMoneyWinner(){
        int countWinner=0;
        for(auto p:player)
            if(p->getWinner())countWinner++;


       int cash =bank/countWinner;
        for(auto p:player){
            if(p->getWinner()){               
                bank -=cash;
                p->addCash(cash);
                protocol.append(p->getName()+" takes "+to_string(cash)+"$");
                protocolTradeRed(protocol);protocol.clear();
            }
        }
        if(bank<0)bank =0;
    }
    string getNameOfChoose(int n){
        switch (n) {
        case 1: return " bets ";
        case 2: return " call ";
        case 3: return " fold ";
        case 4: return " check ";
        }
        return  "";
    }
    pair<int,int> ifPlayerComputer(Player * player){
        if(player->getFold()==false){
            // QThread::sleep(1);
            vector<Card*> cp = commonCards;
            vector<Card*> plCards = player->getCards();
            for(auto c : player->getCards())cp.push_back(c);
            if(cp.size()==2){
                //  cout<<player->getName()<<"  preflop"<<endl;

                return make_pair(2,stavka);
            }
            //           cout<<player->getName()<<" ";
            //            for(auto c : cp)
            //                cout<<c->rang<<" ";
            //            cout<<endl;

            pair<pair<int,int>,string> p=fw.takeCards(plCards,cp);
            //  cout<<"fw.takeCards(plCards,cp); "<<p.second<<endl;
            if(p.second== "High card"){
               // cout<<player->getName()<<" FOLD"<<endl;
                return  make_pair(3,stavka);
            }
            if(p.second== "One pair" || p.second== "Two pair"){
                // cout<<player->getName()<<" CHECK"<<endl;
                if(cp.size()>2 && cp.size()<7){
                //    cout<<player->getName()<<" CHECK"<<endl;
                    return  make_pair(4,stavka);
                }
                else{
                    //   cout<<player->getName()<<" CALL RIVER "<<endl;
                    return  make_pair(2,stavka);
                }

            }

            /// RIASE
            if(player->getMoney()>= stavka+10){
                stavka +=10;
                // cout<<player->getName()<<" RIASE ON "<<stavka<<endl;
                return make_pair(1,stavka);
            }

            //  cout<<player->getName()<<" FOLD the END"<<endl;
            return  make_pair(3,stavka);
            //            cout<<player->getName()<<" "<<p.second
            //               <<p.first.first<<" - "<<p.first.second<<endl;
        }
    }
    string buildHightNameCard(string rang, int cost){
        string str(1,rang.at(0));
        switch (cost) {
        case 14: return str+"Ace";
        case 13: return str+"King";
        case 12: return str+"Lady";
        case 11: return str+"Jack";
        default: return rang;
        }
    }
    void notEnoughMoney(int st){
        if(player.at(0)->getMoney()<st){
            player.at(0)->setFold(true);
            checkFold();
        }
    }

    int bank=0,stavka = 0, lastRichMan,blind=0;
    deque<Player *> player;
    CardDeck * stack;
    vector<Card *> commonCards;
    bool endGame=false;
    QEventLoop _loop;
    pair<int,int> playerChoose;
    string protocol;
    FindWinner fw;
};

#endif // GAME_H
