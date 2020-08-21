#ifndef FINDWINNER_H
#define FINDWINNER_H
#include"Game.h"
//#include <utility>
#include<algorithm>
#include<map>
static vector<Card *> Royal_Flush[4]={{new Card("D14A"),new Card("D13K"),new Card("D12L"),
                                       new Card("D11J"),new Card("D10")},
                                      {new Card("H14A"),new Card("H13K"),new Card("H12L"),
                                       new Card("H11J"),new Card("H10")},
                                      {new Card("C14A"),new Card("C13K"),new Card("C12L"),
                                       new Card("C11J"),new Card("C10")},
                                      {new Card("S14A"),new Card("S13K"),new Card("S12L"),
                                       new Card("S11J"),new Card("S10")}};

class FindWinner{
    pair<int,int> one_pair(vector<Card*> &user){
        sort(user.begin(), user.end(),sort_q);
        map<int,int,greater <int> > countUniq;
        countUniq[user[0]->cost]=1;
        map<int,int>::iterator it;
        int comb2=0;
        for(size_t i=1;i<user.size();i++){
            it = countUniq.find(user[i]->cost);
            if(it == countUniq.end())
                countUniq[user[i]->cost]=1;
            else
                it->second++;
        }
        int maxC = 0;
        for(auto e : countUniq){

            if(e.second==2){
                comb2=e.first;

            }
            if(e.second==1)
                if(maxC<e.first)
                    maxC=e.first;
        }

        if( comb2 && maxC)
            return make_pair(comb2,maxC);

        return make_pair(0,0);
    }
    pair<int,int> two_pair(vector<Card*> &user){
        sort(user.begin(), user.end(),sort_q);
        // show(user);
        map<int,int,greater <int> > countUniq;
        countUniq[user[0]->cost]=1;
        map<int,int>::iterator it;
        int comb2_B=0, comb2_S=0;
        for(size_t i=1;i<user.size();i++){
            it = countUniq.find(user[i]->cost);
            if(it == countUniq.end())
                countUniq[user[i]->cost]=1;
            else
                it->second++;
        }

        for(auto e : countUniq){
            //   cout<<e.first<<" * "<<e.second<<endl;
            if(e.second==2 && comb2_B == 0 )comb2_B=e.first;
            else if(e.second==2 && comb2_B != 0 )comb2_S=e.first;
        }

        if(comb2_B && comb2_S)
            return make_pair(comb2_B,comb2_S);
        return make_pair(0,0);
    }
    pair<int,int> three_of_kind(vector<Card*> &user){
        sort(user.begin(), user.end(),sort_q);
        map<int,int> countUniq;
        countUniq[user[0]->cost]=1;
        map<int,int>::iterator it;
        int comb3=0, maxC=0;
        for(size_t i=1;i<user.size();i++){
            it = countUniq.find(user[i]->cost);
            if(it == countUniq.end())
                countUniq[user[i]->cost]=1;
            else
                it->second++;
        }

        for(auto e :countUniq){
            if(e.second==3)comb3=e.first;
            if(e.second==1)
                if(maxC <e.first)
                    maxC=e.first;
        }
        if(comb3 && maxC)
            return make_pair(comb3,maxC);
        return make_pair(0,0);
    }
    pair<int,int> straight(vector<Card*> &user){
        sort(user.begin(), user.end(),sort_cost);

        int score, checkQueue,counter=1;
        score=checkQueue=user[0]->cost;
        size_t i=1;
        if(user[0]->cost==14 && user[user.size()-1]->cost==2){
            user[0]->cost=1;
            sort(user.begin(), user.end(),sort_cost);

        }
        for(;i<user.size();i++){
            if(checkQueue ==user[i]->cost+1){// проверка последовательности

                score +=user[i]->cost;
                checkQueue--;
                counter++;
                if(counter==5){
                    if( user[0]->cost==6){
                        for(auto c : user)if(c->cost==1)c->cost=14;
                        return   make_pair(score,1); // 6 5 4 3 2
                    }
                    else{
                        for(auto c : user)if(c->cost==1)c->cost=14;
                        return   make_pair(score,0); // 5 4 3 2 A
                    }

                }
            }
            else{
                score=checkQueue=user[i]->cost;
                counter=1;
            }

        }
        for(auto c : user)if(c->cost==1)c->cost=14;
        return make_pair(0,0);
    }
    pair<int,int>flush( vector<Card*> &user){
        sort(user.begin(), user.end(),sort_cost_more);
        // show(user);
        int score=0,counter=0,pos=0;
        size_t i=0;   int maxC=0;
        for(;i<user.size();i++){
            if(user[pos]->rang.at(0) ==user[i]->rang.at(0)){// все одной масти
                counter++;
                if(user[i]->cost>maxC)maxC=user[i]->cost;
                score +=user[i]->cost;
                if(counter==5){
                    return make_pair(maxC,score);
                }
            }else{
                maxC=0;
                pos=i;
                counter=1;
                score =user[i]->cost;
            }
        }
        if(counter==5)
            return make_pair(user[i-4]->cost,score);
        else
            return  make_pair(0,0);

    }
    pair<int,int> full_House( vector<Card*> &user){
        sort(user.begin(), user.end(),sort_q);
        map<int,int> countUniq;
        countUniq[user[0]->cost]=1;
        map<int,int>::iterator it;
        int comb3=0, comb2=0;
        for(size_t i=1;i<user.size();i++){
            it = countUniq.find(user[i]->cost);
            if(it == countUniq.end())
                countUniq[user[i]->cost]=1;
            else
                it->second++;
        }

        for(auto e :countUniq){
            if(e.second==3)comb3=e.first;
            if(e.second==2)comb2=e.first;
        }
        if(comb3 && comb2)
            return make_pair(comb3,comb2);
        return make_pair(0,0);
    }
    int straight_Flush( vector<Card*> &user){
        sort(user.begin(), user.end(),sort_q);
        int score, checkQueue;
        score=checkQueue=user[0]->cost;
        for(size_t i=0;i<user.size();i++){
            if(user[0]->rang.at(0) !=user[i]->rang.at(0)) // все одной масти
                return 0;
            if(checkQueue ==user[i]->cost){// проверка последовательности
                score +=user[i]->cost;
                checkQueue--;
            }
            else
                return 0;
        }
        return score;
    }
    bool eqCards(const vector <Card*> &combo, const vector<Card*> &user){
        size_t counter = combo.size();
        for(size_t i=0;i<user.size();i++)
            for(size_t j=0;j<combo.size();j++)
                if(user[i]->rang==combo[j]->rang)counter--;
        return !counter;
    }
    pair<int,int> four_of_card( vector<Card*> &user){
        sort(user.begin(), user.end(),sort_cost);

        int score, checkQueue,counter =1;;
        score=checkQueue=user[0]->cost;
        for(size_t i=1;i<user.size();i++){
            if(checkQueue ==user[i]->cost){// проверка последовательности
                score +=user[i]->cost;
                counter++;
                if(counter==4){
                    vector<int> cards;
                    for(size_t j=0;j<user.size();j++)
                        if(user[j]->cost!=user[i]->cost){
                            cards.push_back(user[j]->cost);

                        }
                    sort(cards.begin(),cards.end());//поиск старшей из оставших
                    return make_pair(score,cards[cards.size()-1]);
                }

            }
            else{
                checkQueue =user[i]->cost;
                counter=1;
                score =user[i]->cost;
            }
        }
        return make_pair(0,0);

    }
    pair<int,int> countWinners(deque<Player *> &players){
        int maxStatus = 0,counter=0;
        for(auto player : players)
            if(player->getStatusWinner()>maxStatus)
                maxStatus=player->getStatusWinner();

        for(auto player : players)
            if(player->getStatusWinner()== maxStatus && maxStatus>0){
                counter++;
                if(player->getFold()==false)
                     player->setWinner(true);
              //  cout<<"counter winner maxStatus"<<maxStatus<<" " << player->show()<<endl;

            }
            else
                player->setWinner(false);

        return make_pair(counter,maxStatus);
    }
    void setStatusWinnerFromCombo(deque<Player *> &players){
        // int countWinner = 0;
        for(auto player : players){ // поиск победитля по комбо в классе игрока
            if(player->getFold()==false){
                if(player->getCombo()=="Royal Flush")
                    player->setStatusWinner(100);
                else  if(player->getCombo()=="Straight Flush")
                    player->setStatusWinner(99);
                else if(player->getCombo()=="Core")
                    player->setStatusWinner(98);
                else if(player->getCombo()=="Full House")
                    player->setWinner(97);
                else if(player->getCombo()=="Flush")
                    player->setStatusWinner(96);
                else if(player->getCombo()=="Sraight")
                    player->setStatusWinner(95);
                else if(player->getCombo()=="Set")
                    player->setStatusWinner(94);
                else if(player->getCombo()=="Two pair")
                    player->setStatusWinner(93);
                else if(player->getCombo()=="One pair")
                    player->setStatusWinner(92);
                else if(player->getCombo()=="High card")
                    player->setStatusWinner(player->getCombLast().first);

            }else {
               player->setStatusWinner(0);
}
        } //сравниние по рангу вслучае 2 и более одиаковых крат при Hight card



        // поиск Победителя
        pair<int,int> counter_maxSatus =countWinners(players);
        if(counter_maxSatus.first>1){
            for(auto player : players)
                if(player->getFold()==false){
                  //  player->setWinner(0);
                    if(player->getStatusWinner()== counter_maxSatus.second){
                        player->setStatusWinner(player->getCombLast().first);// установка номинала выиграшной карты
                    }else  // избовление от осtальных
                        player->setStatusWinner(0);
                }

            //если камбо карты совпали сравниваем остаток
            counter_maxSatus =countWinners(players);
            if(counter_maxSatus.first>1){
                for(auto player : players)
                    if(player->getFold()==false){
                        player->setWinner(0);
                        if(player->getStatusWinner()== counter_maxSatus.second){

                            player->setStatusWinner(player->getCombLast().second);// установка остаточной карты
                        }else  // избовление от осtальных
                            player->setStatusWinner(0);
                    }

                countWinners(players);  // устанавливаем  победителя и глубже не лезем.... пока
            }
        }

    }
    static inline bool sort_rang(const Card *c1, const Card *c2){
        return (c1->rang > c2->rang );
    }
    static inline bool sort_cost(const Card *c1, const Card *c2){
        return ( c1->cost > c2->cost);
    }
    static inline bool sort_cost_more(const Card *c1, const Card *c2){
        return (c1->rang.at(0) == c2->rang.at(0));
    }
    static inline bool sort_q(const Card *c1, const Card *c2){
        return (c1->id < c2->id);
    }

public:

    void findWinner(vector<Card*> &commonCards,deque<Player *> &players){
        multimap< string, pair<int,int>> result;

        for(auto player : players){
            if(player->getFold()==false){
                vector<Card*> cp = commonCards;
                vector<Card*> plCards = player->getCards();
                for(auto c : player->getCards())cp.push_back(c);
                pair<pair<int,int>,string> p=takeCards(plCards,cp);
                player->setCombo(p.second);
                player->setCombLast(p.first);

            }
        }
        setStatusWinnerFromCombo(players);

    }
    pair<pair<int,int>,string> takeCards(vector<Card*> &user2Cards,vector<Card*> &user){
        //Royal_Flush
        for(int i = 0; i<4;i++)
            if(eqCards(Royal_Flush[i],user))
                return make_pair(make_pair(1,0), string("Royal Flush"));

        //Straight Flush
        int score = straight_Flush(user);
        if(score)
            return make_pair(make_pair(score,0), string("Straight Flush"));
        // CORE
        pair<int,int>  scoreFour = four_of_card(user);
        if(scoreFour.first)
            return make_pair(scoreFour, string("Core"));
        //Full House
        pair<int,int> score3_2 = full_House(user);
        if(score3_2.first)
            return make_pair(score3_2, string("Full House"));

        //Flush     //<старшая, карта остаток>
        pair<int,int> score_flush = flush(user);
        if(score_flush.first)
            return make_pair(score_flush,string("Flush"));

        //Straight // <~, 1 || 0> see straight(user)
        pair<int,int> score_straight =straight(user);
        if(score_straight.first)
            return make_pair(score_straight,string("Sraight"));
        // Сет (Three of a Kind)
        pair<int,int> score_three_of_kind = three_of_kind(user);
        if(score_three_of_kind.first)
            return make_pair(score_three_of_kind,"Set");
        //Two-pair  /// нет проверки 5 карты
        pair<int,int> score_two_pair=two_pair(user);
        if(score_two_pair.first)
            return make_pair(score_two_pair,"Two pair");
        //One-pair
        pair<int,int> score_one_pair=one_pair(user);
        if(score_one_pair.first)
            return make_pair(score_one_pair,"One pair");
        //High-card
        sort(user2Cards.begin(), user2Cards.end(),sort_cost);
        return  make_pair(make_pair(user2Cards[0]->cost,user2Cards[1]->cost),"High card");
    }
    void show(vector<Card *> cards){
        int row=0;
        for(auto c : cards){
            cout<<c->rang<<" "<<"cost "<<c->cost<<" ";row++;
            if(row==5){row=0;cout<<endl;}
        }
        cout<<endl;
    }
};

#endif // FINDWINNER_H
