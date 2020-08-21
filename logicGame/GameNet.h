#ifndef GAMENET_H
#define GAMENET_H
#include "Game.h"
class GameNet: public QObject
{
    Q_OBJECT

signals:
    void protocolTradeBlack(string);
    void protocolTradeGreen(string);
    void protocolTradeRed(string);
    void signalwritePlayersCardNames();
    void signalwriteCommonCards();
    void signalGetPlayerChoose(QString);
public:
     GameNet(){

    }
     vector<Card *>& getCommomCards(){
         return commonCards;
     }
     deque<Player *>& getPlayers(){
         return player;
     }
     int getStavke(){
         return stavka;
     }
     void setStavka(int s){
        stavka=s;
     }
     int getBank(){
         return  bank;
     }
     void addToBank(int n){
         bank +=n;
     }
public slots:
    void  preflop(){

    }
    void commonCardsFromString(QString info){
        qDebug()<<"commonCardsFromString "<<info<<endl;
                QStringList list =  info.split(QRegExp("@"));
                list.removeAll("");
                commonCards.clear();
               for(auto l : list){
                   // qDebug()<<"list cards "<<l<<endl;
                   if(l.size()>30){
                    Card *c = new Card();
                    c->stringFromNet(l);
                    commonCards.push_back(c);
                   }
               }



               switch (commonCards.size()) {
               case 3:
                      protocol = " FLOP ";
                    break;
               case 4:
                      protocol=" TURN ";
                    break;
               case 5:
                     protocol=" RIVER ";
                    break;
               }


               for(int i = 0;i<(int)commonCards.size();i++){
                   protocol.append( buildHightNameCard( commonCards[i]->rang,commonCards[i]->cost)+" ");
               }
 emit  signalwriteCommonCards();
               protocolTradeRed(protocol);protocol.clear();

    }
    QString gameToString(){
       //  bank=:stavka:blind:;  0:1:2:3
        QString str,spt="-";

        str.append(QString::number(bank)+spt).append(QString::number(stavka)+spt)
                .append(QString::number(blind)+spt);
      // commomCards
        spt="+";
        for(auto c :commonCards)
            str.append(c->stringForNet()+spt);
        str.append("-");
      //Players
        spt="@";
        for(auto p : player)
               str.append(p->getInfoPlayer()+spt);

        return  str;
    }
    void stringToGame(QString info){
     //   qDebug()<<"stringToGame client "<<info<<endl;
        QStringList list =  info.split(QRegExp("-"));
        list.removeAll("");
               // for(auto l : list )
                   //  qDebug()<<"stringToGame client "<<l<<endl;
        if(list.size()==4){ // без общих карт
           //  bank=:stavka:blind:;  0:1:2:3
              bank=list.at(0).toInt();
              stavka =list.at(1).toInt();
              blind  = list.at(2).toInt();

            QStringList listPlayers =  list.at(3).split(QRegExp("@"));
              listPlayers.removeAll("");
             player.clear();
              for(auto l : listPlayers ){
                   // qDebug()<<"listPlayers "<<l<<endl;
                  Player * p = new Player();
                  p->setInfoPlaeyr(l);
                //  p->show();
                  player.push_back(p);
              }
         }



  emit signalwritePlayersCardNames();


//        for(auto p :player){
//             if(p->getFold()==false){
//            protocol.append(p->getName()+ " ")
//                    .append(buildHightNameCard(p->getCards()[0]->rang,
//                                              p->getCards()[0]->cost))
//                    .append(" ")
//                    .append(buildHightNameCard(p->getCards()[1]->rang,
//                                              p->getCards()[1]->cost));
//           // protocolTradeBlack(protocol);protocol.clear();
//             }
//        }

    }
    void setPlayerChoose(int button,int stvk){
        playerChoose=make_pair(button,stvk);
    }
      QString getPlayerChoose(){
        QString str;
        str.append(QString::number(playerChoose.first)).append("@").append(QString::number(playerChoose.second));
      //  qDebug()<<"getPlayerChoose() "<<str;
        return  str;

    }
private:
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

#endif // GAMENET_H
