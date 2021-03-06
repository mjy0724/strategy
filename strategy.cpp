#include <ctime>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
typedef long long ll ;
#define rep(i, a, b) for (int i = a ; i <= b ; ++ i)
using namespace std ;

/* vrbs */
// colors : 0 -> gold   1-5 -> green, white, blue, black, red.

/* Table */
    int Round ;
    string PlayerName ;
    struct GEM {
        int color ;
        int count ;
        GEM() {}
        GEM(int _color, int _count) {
            color = _color ;
            count = _count ;
        }
    } ;
    vector <GEM> Gem ;
    struct CARD {
        int level ;
        int score ;
        int color ;
        vector <GEM> cost ;
    } ;
    vector <CARD> Card ;
    struct NOBLE {
        int score ;
        vector <GEM> req ;
    } ;
    vector <NOBLE> Noble ;

/* Player */

    struct PLAYER {
        string name ;
        int score ;
        vector <GEM> gem ;
        vector <CARD> hand ; // purchased cards
        vector <CARD> res ; // reserved cards
        vector <NOBLE> nob ;
    } ;
    vector <PLAYER> Player ;

/* Game */
    int MyID ;
    int CntGem[3][15], CntCol[3][15] ;
    struct OPERATE {
        int type ;
        // 0 : GetGem_Diff
        vector <int> GetGemDiff ;
        // 1 : GetGem_Same
        int GetGemSame ;
        // 2 : Reserve_Card
        CARD ReserveCard ;
        // 3 : Reserve_Card_From_The_Top ;
        int ReserveCardFromTheTop ;
        // 4 : PurchaseCard ;
        CARD PurchaseCard ;
        // 5 : PurchaseReservedCard
        CARD PurchaseReservedCard ;
        // 6 : DesignateNoble
        NOBLE DesignateNoble ;
        int val ;
        bool friend operator < (OPERATE a, OPERATE b) {
            return a.val < b.val ;
        }
    } ;
    vector <OPERATE> ans ;
    OPERATE candidate ;

/* -------------------------------------------------------------------------- */

CARD ReadCard() {
        CARD o ;
        int  _, _x, _y, x , y ;
        scanf("%d%d",  &x, &y) ;
        o.level = x ;
        o.score = y ;
        scanf("%d", &_)  ;
        rep(j, 1, _) {
            scanf("%d%d", &_x, &_y) ;
            o.cost.push_back(GEM(_x, _y)) ;
        }
        return o ;
}

NOBLE ReadNoble() {
        NOBLE o ;
        int  _,  _x, _y,x  ;
        scanf("%d", &x) ;
        o.score = x ;
        scanf("%d",  &_) ;
        rep(j,1 , _) {
            scanf("%d%d", &_x, &_y) ;
            o.req.push_back(GEM(_x, _y)) ;
        }
        return o ;
}

void Read() {

    scanf("%d", &Round) ;
    cin >> PlayerName ;
    int tmp, x, y, _, _x,  _y ;
    scanf("%d", &tmp) ;
    rep(i, 1, tmp) {
        scanf("%d%d", &x, &y) ;
        GEM o ;
        o.color = x ;
        o.count = y ;
        Gem.push_back(o) ;
    }
    scanf("%d", &tmp) ;
    rep(i, 1, tmp)  {
        scanf("%d%d", &x, &y) ;
        CARD o ;
        o.level = x ;
        o.score = y ;
        scanf("%d", &_)  ;
        rep(j, 1, _) {
            scanf("%d%d", &_x, &_y) ;
            o.cost.push_back(GEM(_x, _y)) ;
        }
        Card.push_back(o) ;
    }
    scanf("%d", &tmp) ;
    rep(i, 1, tmp) {
        scanf("%d", &x) ;
        NOBLE o ;
        o.score = x ;
        scanf("%d",  &_) ;
        rep(j,1 , _) {
            scanf("%d%d", &_x, &_y) ;
            o.req.push_back(GEM(_x, _y)) ;
        }
        Noble.push_back(o) ;
    }

    rep(i, 0, 2) {
        cin >> Player[i].name ;
        cin >> Player[i].score ;
        scanf("%d", &tmp) ;
        rep(j, 1, tmp) {
            scanf("%d%d", &x, &y) ;
            Player[i].gem.push_back(GEM(x, y)) ;
        }
        scanf("%d", &tmp) ;
        rep(j, 1, tmp)  Player[i].hand.push_back(ReadCard()) ;
        scanf("%d", &tmp) ;
        rep(j, 1, tmp) Player[i].res.push_back(ReadCard()) ;
        scanf("%d", &tmp) ;
        rep(j,1, tmp) Player[i].nob.push_back(ReadNoble()) ;
    }
}

void Operate() {
    // output vector <OPERATE> ans
}


/* -------------------------------------------------------------------------- */
// 操作遍历及操作判定

int mygems() {
    rep(i, 0, 2) if (Player[i].name == PlayerName) {
        return Player[i].gem.size() ;
    }
    return 0 ;
}

int CANBUY(string name, CARD card) {
    // >= 0 : 需要花费额外的多少个宝石，金宝石算3个
    // < 0 : 距离买它还需要多少个宝石，金宝石算3个
    int cst = 0 ;
    rep(i, 0, 2) if (Player[i].name == name) {
        CARD res = card ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            rep(k, 0, (int) res.cost.size() - 1) if (res.cost[k].color == Player[i].hand[j].color)
                -- res.cost[k].count ;
        }
        rep(j, 0, (int) Player[i].gem.size() - 1) {
            rep(k, 0, (int) res.cost.size() - 1) if (res.cost[k].color == Player[i].gem[j].color) {
                cst += min(res.cost[k].count, Player[i].gem[j].count) ;
                res.cost[k].count -= min(res.cost[k].count, Player[i].gem[j].count) ;
            }
        }
        int ex = 0 ;
        rep(j, 0, (int) Player[i].res.size() - 1) if (res.cost[j].count > 0) ex += res.cost[j].count ;
        if (ex <= Player[i].gem[0].count) {
            return cst + ex * 3 ;
        }
        cst = (ex - Player[i].gem[0].count) * 3 ;
        rep(j, 1, 5) cst += res.cost[j].count ;
        return - cst ;
    }
}

bool CheckNoble(string name, NOBLE noble) {
    rep(i, 0, 2) if (Player[i].name == name) {
        NOBLE res = noble ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            rep(k, 0, (int) res.req.size() - 1) if (res.req[k].color == Player[i].hand[j].color)
                -- res.req[k].count ;
        }
        int ex = 0 ;
        rep(j, 1, 5) if (res.req[j].count > 0) ex += res.req[j].count ;
        return ex == 0 ;
    }
}

int CANNOBLE(string name, NOBLE noble) {
    rep(i, 0, 2) if (Player[i].name == name) {
        NOBLE res = noble ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            rep(k, 0, (int) res.req.size() - 1) if (res.req[k].color == Player[i].hand[j].color)
                -- res.req[k].count ;
        }
        int ex = 0 ;
        rep(j, 1, 5) if (res.req[j].count > 0) ex += res.req[j].count ;
        return ex ;
    }
}

bool CANRESERVE() {
    rep(i, 0, 2) if (PlayerName == Player[i].name) {
        return Player[i].res.size() < 3 ;
    }
}

int CountTheDiffCard(string name, CARD card, int co) {
    rep(i, 0, 2) if (Player[i].name == name) {
        CARD res = card ;
        int sum = 0 ;
        rep(k, 0, (int) res.cost.size() - 1) if (res.cost[k].color == co) sum = res.cost[k].count ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            if (Player[i].hand[j].color != co) continue ;
            sum -- ;
        }
        rep(j, 0, (int) Player[i].gem.size() - 1) {
            if (Player[i].gem[j].color != co) continue ;
            sum -= Player[i].gem[j].count ;
        }
        sum -= Player[i].gem[0].count ;
        return max(sum, 0) ;
    }
}

int CountTheDiffNoble(string name, NOBLE Noble, int co) {
    rep(i, 0, 2) if (Player[i].name == name) {
        NOBLE res = Noble ;
        int sum = 0 ;
        rep(k, 0, (int) res.req.size() - 1) if (res.req[k].color == co) sum = res.req[k].count ;
        rep(j, 0, (int) Player[i].hand.size() - 1) {
            if (Player[i].hand[j].color != co) continue ;
            sum -- ;
        }
        return max(sum, 0) ;
    }
}

void TryGetGemDiff(int w, vector <int> val) { // val [0~30]
    if (10 - mygems() >= 1) {
        rep(i, 0, 5) if (Gem[i].count && val[Gem[i].color]) {
            OPERATE o ;
            o.type = 0 ;
            o.val = val[Gem[i].color] ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            if (candidate < o) candidate = o ;
        }
    }
    if (10 - mygems() >= 2) {
        rep(i, 0, 5) rep(j, i + 1, 5) if (Gem[i].count && Gem[j].count && val[Gem[i].color] && val[Gem[j].color]) {
            OPERATE o ;
            o.type = 0 ;
            o.val = val[Gem[i].color] + val[Gem[j].color] ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            o.GetGemDiff.push_back(j) ;
            if (candidate < o) candidate = o ;

        }
    }
    if (10 - mygems() >= 3) {
        rep(i, 0, 5) rep(j, i + 1, 5) rep(k, j + 1, 5) if (Gem[i].count && Gem[j].count && val[Gem[i].color] && val[Gem[j].color] && Gem[k].count && val[Gem[k].color]) {
            OPERATE o ;
            o.type = 0 ;
            o.val = val[Gem[i].color] + val[Gem[j].color] + val[Gem[k].color] ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            o.GetGemDiff.push_back(j) ;
            o.GetGemDiff.push_back(k) ;
            if (candidate < o) candidate = o ;

        }
    }
}

void TryGetGemSame(int w, vector <int> val) { // val [0~30]
    if (10 - mygems() >= 2) {
        rep(i, 0, 5) if (Gem[i].count > 1 && val[Gem[i].color]) {
            OPERATE o ;
            o.type = 1 ;
            o.val = val[Gem[i].color] * 2 ;
            o.val *= w ;
            o.GetGemDiff.push_back(i) ;
            if (candidate < o) candidate = o ;
        }
    }
}

void ReserveCard(int wscore, vector <int> wcolor) {
    if (!CANRESERVE()) return ;
    rep(i, 0, (int) Card.size() - 1) {
        OPERATE o ;
        o.type = 2 ;
        o.val = Card[i].score * wscore + wcolor[Card[i].color] ;
        o.PurchaseCard = Card[i] ;
        o.val /= (CANBUY(PlayerName, Card[i]) + 1) * (CANBUY(PlayerName, Card[i]) + 1) ;
        if (candidate < o) candidate = o ;
    }
}

void ReserveCardFromTheTop(int w) {
    OPERATE o ;
    o.type = 3 ;
    o.val = w ;
    o.ReserveCardFromTheTop = 2 ;
    if (candidate < o) candidate = o ;
}

void PurchaseCard(int wscore, vector <int> wcolor) { // wscore [100 ~ 500]   wcolor [50~100]
    rep(i, 0, (int) Card.size() - 1) {
        int tmp = CANBUY(PlayerName, Card[i]) ;
        if (tmp >= 0) {
            OPERATE o ;
            o.type = 4 ;
            o.val = Card[i].score * wscore + wcolor[Card[i].color] ;
            o.val /= tmp ;
            o.PurchaseCard = Card[i] ;
            if (candidate < o) candidate = o ;
        }
    }
}

void PurchaseReservedCard(int wscore, vector <int> wcolor) {
    rep(id, 0, 2) if (Player[id].name == PlayerName) {
        rep(i, 0, (int) Player[id].res.size() - 1) {
            CARD now = Player[id].res[i] ;
            int tmp = CANBUY(PlayerName, now) ;
            if (tmp >= 0) {
                OPERATE o ;
                o.type = 5 ;
                o.val = now.score * wscore + wcolor[now.color] ;
                o.val /= tmp ;
                o.PurchaseReservedCard = now ;
                if (candidate < o) candidate = o ;
            }
        }
    }
}

void DesignateNoble() {
    rep(i, 0, (int) Noble.size() - 1) {
        if (CheckNoble(PlayerName, Noble[i])) {
            OPERATE o ;
            o.type = 6 ;
            o.DesignateNoble = Noble[i] ;
            ans.push_back(o) ;
            return ;
        }
    }
}

/* -------------------------------------------------------------------------- */
// 发展经济时期策略


void Develop() {
    // Evaluate Gems
    vector <int> ValGem ;
    int mx = 0 ;
    rep(i, 1, 5) mx = max(mx, CntGem[MyID][i]) ;
    mx ++ ;
    int base = 1 ;
    rep(i, 1, 5) base = max(base, mx - CntGem[MyID][i]) ;
    ValGem.push_back(0) ;
    rep(i, 1, 5) {
        ValGem.push_back((int) (30.0 * (mx - CntGem[MyID][i]) / base) ) ;
    }
    TryGetGemDiff(30, ValGem) ;
    TryGetGemSame(30, ValGem) ;
    // Evaluate color
    vector <int> ValCol ;
    mx = 0 ;
    rep(i, 1, 5) mx = max(mx, CntCol[MyID][i]) ;
    mx ++ ;
    base = 1 ;
    rep(i, 1, 5) base = max(base, mx - CntCol[MyID][i]) ;
    ValCol.push_back(0) ;
    rep(i, 1, 5) {
        ValCol.push_back((int) (100.0 * (mx - CntCol[MyID][i]) / base) ) ;
    }
    ReserveCard(5, ValCol) ;
    ReserveCardFromTheTop(5) ;
    PurchaseCard(100, ValCol) ;
    PurchaseReservedCard(150, ValCol) ;
    ans.push_back(candidate) ;
    DesignateNoble() ;
}

/* -------------------------------------------------------------------------- */
// 抢分时期策略

vector <int> EvaluateColor() {
    vector <int> res ;
    rep(i, 0, 5) res.push_back(0) ;
    rep(i, 0, (int) Card.size() - 1) {
        int tmp = CANBUY(PlayerName, Card[i]) ;
        if (tmp >= 0 || tmp < - 5) continue ;
        rep(j, 1, 5) res[j] += CountTheDiffCard(PlayerName, Card[i], j) * (tmp + 6) * (tmp + 6) ;
    }
    rep(i, 0, (int) Noble.size() - 1) {
        int tmp = CANNOBLE(PlayerName, Noble[i]) ;
        if (tmp >= 0 || tmp < - 5) continue ;
        rep(j, 1, 5) res[j] += CountTheDiffNoble(PlayerName, Noble[i], j) * (tmp + 6) * (tmp + 6) * 3 ;
    }
    int mx = 1 ;
    rep(i, 1, 5) mx = max(mx, res[i]) ;
    rep(i, 1, 5) res[i] = (int) (30.0 * res[i] / mx) ;
    return res ;
}

void Aim() {// Evaluate Gems
    vector <int> ValCol = EvaluateColor() ;
    TryGetGemDiff(10, ValCol) ;
    TryGetGemSame(10, ValCol) ;
    // Evaluate color
    rep(i, 0, 5) ValCol[i] *= 2 ;
    ReserveCard(200, ValCol) ;
    ReserveCardFromTheTop(50) ;
    PurchaseCard(500, ValCol) ;
    PurchaseReservedCard(300, ValCol) ;
    ans.push_back(candidate) ;
    DesignateNoble() ;
}

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
// 舒适时期策略

bool CheckStatus(string name) {
    // check 这个人在当前局面下能不能赢，改一改可获得这个人在当前局面下能获得多少分
    rep(id, 0, 2) if (Player[id].name == name) {
        if (Player[id].score >= 15) return false ;
        rep(i, 0, (int) Card.size() - 1) {
            if (CANBUY(name, Card[i]) && Player[id].score + Card[i].score >= 15) return true ;
        }
        rep(i, 0, (int) Player[id].res.size() - 1) {
            if (CANBUY(name, Player[id].res[i]) && Player[id].score + Player[id].res[i].score >= 15) return true ;
        }
        rep(i, 0, (int) Noble.size() - 1) {
            if (CheckNoble(name, Noble[i]) && Player[id].score + Noble[i].score >= 15) return true ;
        }
    }
    return false ;
}

void GoAndGetIt() {
    rep(id, 0, 2) if (Player[id].name == PlayerName) {
        rep(i, 0, (int) Card.size() - 1) {
            if (CANBUY(PlayerName, Card[i]) && Player[id].score + Card[i].score >= 15) {
                OPERATE o ;
                o.type = 4 ;
                o.PurchaseCard = Card[i] ;
                ans.push_back(o) ;
                DesignateNoble() ;
                return ;
            }
        }
        rep(i, 0, (int) Player[id].res.size() - 1) {
            if (CANBUY(PlayerName, Player[id].res[i]) && Player[id].score + Player[id].res[i].score >= 15) {
                OPERATE o ;
                o.type = 5 ;
                o.PurchaseReservedCard = Player[id].res[i] ;
                ans.push_back(o) ;
                DesignateNoble() ;
                return ;
            }
        }
        rep(i, 0, (int) Noble.size() - 1) {
            if (CheckNoble(PlayerName, Noble[i]) && Player[id].score + Noble[i].score >= 15) {
                OPERATE o ;
                o.type = 6 ;
                o.DesignateNoble = Noble[i] ;
                ans.push_back(o) ;
                return ;
            }
        }
    }
}

void Pre() {
    rep(id, 0, 2) {
        if (PlayerName == Player[id].name) MyID = id ;
        rep(i, 0, 10) CntGem[id][i] = 0 ;
        rep(i, 0, (int) Player[id].gem.size() - 1) {
            CntGem[id][Player[id].gem[i].color] += Player[id].gem[i].count ;
        }
    }
    rep(id, 0, 2) {
        rep(i, 0, 10) CntCol[id][i] = 0 ;
        rep(i, 0, (int) Player[id].hand.size() - 1) {
            CntCol[id][Player[id].hand[i].color] ++ ;
        }
    }
}

void Solve() {
    Pre() ;
    candidate.val = - 1 ;
    candidate.type = - 1 ;
    if (Round <= 10) {
        if (rand() & 1) Develop() ;
            else Aim() ;
    } else {
        if (CheckStatus(PlayerName)) {
            GoAndGetIt() ;
        } else {
            /*bool flg = true ;
            rep(i, 0, 2) if (Player[i].name != PlayerName && CheckStatus(Player[i].name)) {
                Block() ;
                flg = false ;
                break ;
            }
            if (flg) {
                Aim() ;
            }*/
            Aim() ;
        }
    }
}

int main() {
    srand(time(NULL)) ;
    Read() ;
    Solve() ;
    // add to the list, and push in Operate()
    Operate() ;
    return 0 ;
}
