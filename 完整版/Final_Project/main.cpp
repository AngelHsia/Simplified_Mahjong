#include <SDL2/SDL.h>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <string>
#include <iostream>

#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <random>
#include <ctime>

using namespace std;

enum suit{Tong, Line, Wan, East, South, West, North, Cheng, Fa, Bai, NONE};

struct card{
    suit type;
    int value;
    int num;
    string picture;
    card operator=(const card a);
};

card card::operator=(const card ori){
    card newone;
    newone.num=ori.num;
    newone.value=ori.value;
    newone.type=ori.type;
    newone.picture=ori.picture;
    return newone;
}

string pic[10][10]=
        {{"","./CARDS/Tong1.png","./CARDS/Tong2.png","./CARDS/Tong3.png","./CARDS/Tong4.png","./CARDS/Tong5.png","./CARDS/Tong6.png","./CARDS/Tong7.png","./CARDS/Tong8.png","./CARDS/Tong9.png"},
         {"","./CARDS/Line1.png","./CARDS/Line2.png","./CARDS/Line3.png","./CARDS/Line4.png","./CARDS/Line5.png","./CARDS/Line6.png","./CARDS/Line7.png","./CARDS/Line8.png","./CARDS/Line9.png"},
         {"","./CARDS/Wan1.png","./CARDS/Wan2.png","./CARDS/Wan3.png","./CARDS/Wan4.png","./CARDS/Wan5.png","./CARDS/Wan6.png","./CARDS/Wan7.png","./CARDS/Wan8.png","./CARDS/Wan9.png"},
         {"./CARDS/East.png"},
         {"./CARDS/South.png"},
         {"./CARDS/West.png"},
         {"./CARDS/North.png"},
         {"./CARDS/Cheng.png"},
         {"./CARDS/Fa.png"},
         {"./CARDS/Bai.png"}
        };
string full_pic[10][10] =
        {{"", "./Full_CARDS/Tong1.png", "./Full_CARDS/Tong2.png", "./Full_CARDS/Tong3.png", "./Full_CARDS/Tong4.png", "./Full_CARDS/Tong5.png", "./Full_CARDS/Tong6.png", "./Full_CARDS/Tong7.png", "./Full_CARDS/Tong8.png", "./Full_CARDS/Tong9.png"},
         {"", "./Full_CARDS/Line1.png", "./Full_CARDS/Line2.png", "./Full_CARDS/Line3.png", "./Full_CARDS/Line4.png", "./Full_CARDS/Line5.png", "./Full_CARDS/Line6.png", "./Full_CARDS/Line7.png", "./Full_CARDS/Line8.png", "./Full_CARDS/Line9.png"},
         {"", "./Full_CARDS/Wan1.png",  "./Full_CARDS/Wan2.png",  "./Full_CARDS/Wan3.png",  "./Full_CARDS/Wan4.png",  "./Full_CARDS/Wan5.png",  "./Full_CARDS/Wan6.png",  "./Full_CARDS/Wan7.png",  "./Full_CARDS/Wan8.png",  "./Full_CARDS/Wan9.png"},
         {"./Full_CARDS/East.png"},
         {"./Full_CARDS/South.png"},
         {"./Full_CARDS/West.png"},
         {"./Full_CARDS/North.png"},
         {"./Full_CARDS/Cheng.png"},
         {"./Full_CARDS/Fa.png"},
         {"./Full_CARDS/Bai.png"}
        };

const int SCREEN_WIDTH = 640 * 2;
const int SCREEN_HEIGHT = 480 * 2;

int Board_num = 0;//桌上的第幾張牌
int first_num = 0;//桌上的第幾張牌
int second_num = 0;//桌上的第幾張牌
int third_num = 0;//桌上的第幾張牌
SDL_Rect CardSize = {0, 0, 60, 75};

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Surface *gSurface = NULL;
SDL_Texture *texture = NULL;
SDL_Event event;

void SDL_Delay(Uint32 ms);

class LButton {
public:
    LButton();

    ~LButton();

    void setPosition(int x, int y);

    void LoadFromFile(std::string path);

    void render(SDL_Renderer *renderer);

    void free();

    bool handleEvent();

protected:
    SDL_Point centerP;
    SDL_Texture *B_texture;
    SDL_Rect destination;
};

class aCard : public LButton {
public:
    aCard();

    void setCard(suit t, int v);

    void clean_card();

    suit getType();

    int getValue();

    void go_Board(bool reset);

    aCard operator=(const aCard *a);

    void load_sets_1(aCard a,aCard b,bool reset=0);
    void load_sets_2(aCard a,aCard b,bool reset=0);
    void load_sets_3(aCard a,aCard b,bool reset=0);

private:
    suit type;
    int value;
    int Board_inx = 360, Board_iny = 370;
};
class HandCards : public LButton {
public:
    HandCards();

    aCard C_Sequence[17];

    void placeCards();

private:
    int StillIn;
};

void loadPage(std::string path);


void loadTexture(std::string path, int centerPos_x, int centerPos_y);
aCard::aCard() {
    type = NONE;
    value = 0;
}
void aCard::setCard(suit t, int v = 0) {
    type = t;
    value = v;
    string source = full_pic[t][v];
    LoadFromFile(source);
}

suit aCard::getType() {
    suit t = type;
    return t;
}

int aCard::getValue() {
    int v = value;
    return v;
}

void aCard::clean_card() {
    type = NONE;
    value = 0;
}

void aCard::go_Board(bool reset=false) {
    static aCard* Cards_on_board;
    static int Board_row = 1;//桌上的第幾列
    static int Board_col = 1;//桌上的第幾欄

    if(reset||Board_num==0){//重新開始遊戲的話
        Board_num = 0;//桌上的第幾張牌
        Board_row = 1;//桌上的第幾列
        Board_col = 1;//桌上的第幾欄
    }

    string source = full_pic[type][value];
    LoadFromFile(source);
    destination.w*=0.5;
    destination.h*=0.5;
    Board_num++;
    cout << "Board_num=" << Board_num << "; chosen (Value,Type)=" << "(" << value << ", " << type << ")\n";
    if(Cards_on_board==NULL)
        Cards_on_board = new aCard [Board_num];
    else{
        aCard* temp=new aCard[Board_num];
        for (int i = 0; i < Board_num-1; ++i) {
            temp[i]=Cards_on_board[i];
            cout<<"1st Cards_on_board["<<i<<"] (Value,Type)=" << "(" << Cards_on_board[i].value << ", " << Cards_on_board[i].type << ")\n";
        }

        delete [] Cards_on_board;
        Cards_on_board =temp;
        for (int i = 0; i < Board_num-1; ++i) {
            cout<<"2nd Cards_on_board["<<i<<"] (Value,Type)=" << "(" << Cards_on_board[i].value << ", " << Cards_on_board[i].type << ")\n";
        }
    }



    int temp_x, temp_y;
    temp_x = Board_inx + (Board_col - 1) * destination.w;
    temp_y = Board_iny + (Board_row - 1) * destination.h;
    cout << "Board_row=" << (Board_row) << ", (Board_col)=" << Board_col << endl;
    cout << "Board_inx + (Board_col - 1) * destination.w = " << Board_inx << "+" << (Board_col - 1) << "*"
         << destination.w << "\n";
    cout << "Board_iny + (Board_row - 1) * destination.h = " << Board_iny << "+" << (Board_row - 1) << "*"
         << destination.h << "\n";
    this->setPosition(temp_x, temp_y);
    Cards_on_board[Board_num-1]=this;
    loadPage("./mytest/2nd page_3.png");
    loadTexture("./mytest/mahjong.png", SCREEN_WIDTH / 2, SCREEN_HEIGHT * 2 / 3);
    for (int i = 0; i < Board_num; ++i) {
        cout<<"3rd Cards_on_board["<<i<<"] (Value,Type)=" << "(" << Cards_on_board[i].value << ", " << Cards_on_board[i].type << ")\n";
        string source=full_pic[Cards_on_board[i].type][Cards_on_board[i].value];
        Cards_on_board[i].LoadFromFile(source);
        Cards_on_board[i].destination.w*=0.5;
        Cards_on_board[i].destination.h*=0.5;
        Cards_on_board[i].render(gRenderer);
    }

    if (Board_num % 20 == 0) {
        Board_row++;
        Board_col = 1;
    } else {
        Board_col++;
    }


}

aCard aCard:: operator=(const aCard *target) {
    destination=target->destination;
    centerP=target->centerP;
    B_texture =target->B_texture;

    type = target->type;
    value = target->value;

    return  *this;
}

void transform_card_to_CSeq(card **cardsInHand, HandCards &players);

HandCards::HandCards() {
    StillIn = 16;
}

void HandCards::placeCards() {
    for (int i = 0; i < 17; ++i) {
        C_Sequence[i].setPosition(190 + i * CardSize.w, 647);
        C_Sequence[i].render(gRenderer);
        C_Sequence[i].render(gRenderer);
    }
}

void loadTexture(std::string path, int centerPos_x, int centerPos_y) {
    SDL_Rect destination;
    SDL_Surface *loadedSurf = IMG_Load(path.c_str());
    if (loadedSurf == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurf);
    if (texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
    }
    destination.w = loadedSurf->w;
    destination.h = loadedSurf->h;
    destination.x = (centerPos_x - destination.w / 2);
    destination.y = (centerPos_y - destination.h / 2);
    //cout<<"destination(w,h,x,y)="<<"("<<destination.w<<","<<destination.h<<","<<destination.x<<","<<destination.y<<")\n";
    SDL_FreeSurface(loadedSurf);
    SDL_RenderCopy(gRenderer, texture, NULL, &destination);
    return;
}

void close();


LButton::LButton() {
    centerP.x = 0;
    centerP.y = 0;
    B_texture = NULL;
    destination.x = 0;
    destination.y = 0;
    destination.w = 0;
    destination.h = 0;
    return;
}

LButton::~LButton() {
    free();
}

void LButton::free() {
    //Free texture if it exists
    if (B_texture != NULL) {
        SDL_DestroyTexture(B_texture);
        B_texture = NULL;
        destination.w = 0;
        destination.h = 0;
    }
}


void LButton::setPosition(int x, int y) {
    centerP.x = x;
    centerP.y = y;
    destination.x = (centerP.x - destination.w / 2);
    destination.y = (centerP.y - destination.h / 2);
    cout << "Reset des(w,h,x,y)=" << "(" << destination.w << "," << destination.h << "," << destination.x << ","
         << destination.y << ")\n";
}

void LButton::LoadFromFile(std::string path) {
    free();
    SDL_Surface *loadedSurf = IMG_Load(path.c_str());
    if (loadedSurf == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return;
    }
    B_texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurf);
    if (B_texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
    }

    destination.w = loadedSurf->w;
    destination.h = loadedSurf->h;
    //cout<<"destination(w,h,x,y)="<<"("<<destination.w<<","<<destination.h<<","<<destination.x<<","<<destination.y<<")\n";
    SDL_FreeSurface(loadedSurf);
}


void LButton::render(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, B_texture, NULL, &destination);
}

bool LButton::handleEvent() {
    //If mouse event happened

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        //Get mouse position
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);
        //Check if mouse is in button
        bool inside;

        //Mouse is left of the button
        if (x < destination.x) {
            inside = false;
        }
            //Mouse is right of the button
        else if (x > destination.x + destination.w) {
            inside = false;
        }
            //Mouse above the button
        else if (y < destination.y) {
            inside = false;
        }
            //Mouse below the button
        else if (y > destination.y + destination.h) {
            inside = false;
        } else { inside = true; }
        /*if(inside)
            cout<<"inside="<<inside;
        cout<<"proper range(x1-->x2,y1-->y2)="<<"("<<destination.x<<"-->"<<destination.x+destination.w<<" , ";
        cout<<destination.y<<"-->"<<destination.y+destination.h<<")\n";
        cout<<"(x,y)="<<"("<<x<<" , "<<y<<")\n";*/
        return inside;
    }
    return false;
}


void close() {
    //Free loaded images
    SDL_DestroyTexture(texture);
    //Destroy gWindow
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void loadPage(std::string path) {
    texture = NULL;
    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, texture, NULL, NULL);
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return;
}


void aCard::load_sets_1(aCard a,aCard b,bool reset) {
    static int Board_row = 1;//第幾列
    static int Board_col = 1;//第幾欄
    const int set1_inx=180,set1_iny=50;

    static aCard *first_set= nullptr;
    if(reset|| first_num ==0){//重新開始遊戲的話
        first_num = 0;
        Board_row = 1;
        Board_col = 1;
    }
    //aㄉ
    first_num++;
    if(first_set==NULL)
        first_set = new aCard [first_num+2];
    else{
        aCard* temp=new aCard[first_num+2];
        for (int i = 0; i < first_num + 1; ++i) {
            temp[i]=first_set[i];
            }delete [] first_set;
        first_set =temp;
    }
    string source = full_pic[a.type][a.value];
    a.LoadFromFile(source);
    a.destination.w*=0.5;
    a.destination.h*=0.5;

    int temp_x, temp_y;
    temp_x = set1_inx + (Board_col - 1) * a.destination.w;
    temp_y = set1_iny + (Board_row - 1) * a.destination.h;
    a.setPosition(temp_x, temp_y);
    first_set[first_num - 1]=a;
    Board_col++;

///      換自己這張牌
    first_num++;
    source = full_pic[type][value];
    LoadFromFile(source);
    destination.w*=0.5;
    destination.h*=0.5;

    temp_x = set1_inx + (Board_col - 1) * destination.w;
    temp_y = set1_iny + (Board_row - 1) * destination.h;

    setPosition(temp_x, temp_y);
    first_set[first_num - 1]=this;

    Board_col++;

/// Bㄉ那張
    first_num++;
    source = full_pic[b.type][b.value];
    b.LoadFromFile(source);
    b.destination.w*=0.5;
    b.destination.h*=0.5;

    temp_x = set1_inx + (Board_col - 1) * b.destination.w;
    temp_y = set1_iny + (Board_row - 1) * b.destination.h;

    b.setPosition(temp_x, temp_y);
    first_set[first_num - 1]=b;

    source=full_pic[b.type][b.value];
    for (int i = 0; i < first_num; ++i) {
        source=full_pic[first_set[i].type][first_set[i].value];
        first_set[i].LoadFromFile(source);
        first_set[i].destination.w*=0.5;
        first_set[i].destination.h*=0.5;
        first_set[i].render(gRenderer);
    }

    if (first_num % 3 == 0) {
        Board_row++;
        Board_col = 1;
    } else {
        Board_col++;
    }
    return;

}
void aCard::load_sets_2(aCard a,aCard b,bool reset) {
    static int Board_row = 1;//第幾列
    static int Board_col = 1;//第幾欄
    const int set1_inx=430,set1_iny=50;

    static aCard *second_set= nullptr;
    if(reset|| second_num==0){ //重新開始遊戲的話
        second_num = 0;
        Board_row = 1;
        Board_col = 1;
    }
    //aㄉ
    second_num++;
    if(second_set == NULL)
        second_set = new aCard [second_num + 2];
    else{
        aCard* temp=new aCard[second_num + 2];
        for (int i = 0; i < second_num + 1; ++i) {
            temp[i]=second_set[i];
        }delete [] second_set;
        second_set =temp;
    }
    string source = full_pic[a.type][a.value];
    a.LoadFromFile(source);
    a.destination.w*=0.5;
    a.destination.h*=0.5;

    int temp_x, temp_y;
    temp_x = set1_inx + (Board_col - 1) * a.destination.w;
    temp_y = set1_iny + (Board_row - 1) * a.destination.h;
    a.setPosition(temp_x, temp_y);
    second_set[second_num - 1]=a;
    Board_col++;

///      換自己這張牌
    second_num++;
    source = full_pic[type][value];
    LoadFromFile(source);
    destination.w*=0.5;
    destination.h*=0.5;

    temp_x = set1_inx + (Board_col - 1) * destination.w;
    temp_y = set1_iny + (Board_row - 1) * destination.h;

    setPosition(temp_x, temp_y);
    second_set[second_num - 1]=this;
    Board_col++;

/// Bㄉ那張
    second_num++;
    source = full_pic[b.type][b.value];
    b.LoadFromFile(source);
    b.destination.w*=0.5;
    b.destination.h*=0.5;

    temp_x = set1_inx + (Board_col - 1) * b.destination.w;
    temp_y = set1_iny + (Board_row - 1) * b.destination.h;

    b.setPosition(temp_x, temp_y);
    second_set[second_num - 1]=b;

    source=full_pic[b.type][b.value];

    for (int i = 0; i < second_num; ++i) {
        source=full_pic[second_set[i].type][second_set[i].value];
        second_set[i].LoadFromFile(source);
        second_set[i].destination.w*=0.5;
        second_set[i].destination.h*=0.5;
        second_set[i].render(gRenderer);
    }

    if (second_num % 3 == 0) {
        Board_row++;
        Board_col = 1;
    } else {
        Board_col++;
    }
    return;

}
void aCard::load_sets_3(aCard a,aCard b,bool reset) {

    static int Board_row = 1;//桌上的第幾列
    static int Board_col = 1;//桌上的第幾欄
    const int set1_inx=SCREEN_WIDTH*3/4,set1_iny=180;

    static aCard *third_set= nullptr;
    if(reset|| third_num==0){//重新開始遊戲的話
        third_num = 0;//桌上的第幾張牌
        Board_row = 1;//桌上的第幾列
        Board_col = 1;//桌上的第幾欄
    }
    //aㄉ
    third_num++;
    if(third_set == NULL)
        third_set = new aCard [third_num + 2];
    else{
        aCard* temp=new aCard[third_num + 2];
        for (int i = 0; i < third_num + 1; ++i) {
            temp[i]=third_set[i];
        }delete [] third_set;
        third_set =temp;
    }
    string source = full_pic[a.type][a.value];
    a.LoadFromFile(source);
    a.destination.w*=0.5;
    a.destination.h*=0.5;

    int temp_x, temp_y;
    temp_x = set1_inx + (Board_col - 1) * a.destination.w;
    temp_y = set1_iny + (Board_row - 1) * a.destination.h;
    a.setPosition(temp_x, temp_y);
    third_set[third_num - 1]=a;
    Board_col++;

///      換自己這張牌
    third_num++;
    source = full_pic[type][value];
    LoadFromFile(source);
    destination.w*=0.5;
    destination.h*=0.5;

    temp_x = set1_inx + (Board_col - 1) * destination.w;
    temp_y = set1_iny + (Board_row - 1) * destination.h;

    setPosition(temp_x, temp_y);
    third_set[third_num - 1]=this;
    Board_col++;

/// Bㄉ那張
    third_num++;
    source = full_pic[b.type][b.value];
    b.LoadFromFile(source);
    b.destination.w*=0.5;
    b.destination.h*=0.5;

    temp_x = set1_inx + (Board_col - 1) * b.destination.w;
    temp_y = set1_iny + (Board_row - 1) * b.destination.h;

    b.setPosition(temp_x, temp_y);
    third_set[third_num - 1]=b;

    source=full_pic[b.type][b.value];

    for (int i = 0; i < third_num; ++i) {
        source=full_pic[third_set[i].type][third_set[i].value];
        third_set[i].LoadFromFile(source);
        third_set[i].destination.w*=0.5;
        third_set[i].destination.h*=0.5;
        third_set[i].render(gRenderer);
    }

    if (third_num % 3 == 0) {
        Board_row++;
        Board_col = 1;
    } else {
        Board_col++;
    }
    return;

}





//...................................\\//....................................\\//
int Fixed_Card;
card Remain_cards[10][10];
struct CHI{
    int  chiType;             //牌type
    int  chiValue[2];         //吃牌組合
};

class gamer{
public:
    gamer();
    card ** cards_inhand;
    CHI chiconditions[3];
    void deal(); //發牌
    //void draw_card(); //抽牌
    
    bool checkchi(card a); //每次重檢查是否能吃
    bool checkpeng(card a);//每次重檢查是否能碰
    bool checkHu(card a);//檢查別人有沒有放槍
    bool checkHu();//檢查自己有沒有胡
    bool well_formed(card**,card**);
    bool chi_or_peng;//有沒有吃或碰過

    void printchi(card a);//print out all chi choices
    void printpeng(card a);//print out all peng choices

    void print_cards_inhand(); //debug

protected:
    static int NUMBER_OF_CARDS; //手牌總數
    bool canchi;
    bool canpeng;
    bool haschi;
    bool haspeng;
};

int gamer::NUMBER_OF_CARDS = 16;

gamer::gamer(){
    canchi = false;
    canpeng = false;
    chi_or_peng = false;

    cards_inhand = new card *[10];
    for(int i =0; i < 10; i++){
        cards_inhand[i] = new card[10];
    }

    haspeng = 0;
    haschi = 0;
}

void gamer::print_cards_inhand(){
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            if(cards_inhand[i][j].num != 0){
                cout << cards_inhand[i][j].type << ',' << cards_inhand[i][j].value << ',' << cards_inhand[i][j].num << '\n';
            }
        }
    }
}

void gamer::deal(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 33);
    int ran=0;
    for(int i=0 ; i<16 ; i++){
        ran = dis(gen);
        //cout << ran << '\n';
        if(ran<27){      //一般的牌
            int a = int(ran/9);   //Tong or Line or Wan
            int b = int(ran%9)+1;   //value

            if(Remain_cards[a][b].num==0 or cards_inhand[a][b].num>=3){
                i--;
            }
            else{
                cards_inhand[a][b].num++;
                Remain_cards[a][b].num--;
                //cout << "(a , b) = " << '(' << a <<" , "<< b << ")\n";
            }
        }
        else{            //風中發白  //ran=27~33
            int c = ran-24;

            if(Remain_cards[c][0].num==0 or cards_inhand[c][0].num>=3){
                i--;
            }
            else{
                cards_inhand[c][0].num++;
                Remain_cards[c][0].num--;
            }
        }
    }
    //print_cards_inhand();
}

bool gamer::checkHu(card a){   //別人出牌時檢查他有沒有放槍
    cout << "fake checkhu start~\n";
    bool result=0;
    cards_inhand[a.type][a.value].num++; //假裝拿到那張牌

    card** cards_copy_real;
    cards_copy_real = new card *[10];
    for(int i=0 ; i<10 ; i++){
        cards_copy_real[i] = new card[10];
    }

    for(int i=0 ; i<10 ; i++){ //複製一個手牌
        for(int j=0 ; j<10 ; j++){
            cards_copy_real[i][j].type = cards_inhand[i][j].type;
            cards_copy_real[i][j].value = cards_inhand[i][j].value;
            cards_copy_real[i][j].num = cards_inhand[i][j].num;
            cards_copy_real[i][j].picture = cards_inhand[i][j].picture;
            //cards_copy_real[i][j]=*( &cards_inhand[i][j]);
        }
    }

    card** cards_copy;
    cards_copy = new card *[10];
    for(int i =0; i < 10; i++){
        cards_copy[i] = new card[10];
    }

    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            cards_copy[i][j] = cards_inhand[i][j];
            cards_copy[i][j].num = 0;
        }
    }
    result = well_formed(cards_copy_real, cards_copy);
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            cards_inhand[i][j] = (cards_copy_real[i][j]);
        }
    }
    cards_inhand[a.type][a.value].num--; //移除假裝拿到ㄉ那張牌
    return result;
}

bool gamer::checkHu(){    //自己抽牌時看有沒有自摸
    cout << "checkhu start~\n";
    bool result=0;
    card** cards_copy;
    cards_copy = new card *[10];
    for(int i=0 ; i<10 ; i++){
        cards_copy[i] = new card[10];
    }
    card** cards_copy_real;
    cards_copy_real = new card *[10];
    for(int i=0 ; i<10 ; i++){
        cards_copy_real[i] = new card[10];
    }

    for(int i=0 ; i<10 ; i++){ //複製一個手牌
        for(int j=0 ; j<10 ; j++){
            cards_copy_real[i][j].type = cards_inhand[i][j].type;
            cards_copy_real[i][j].value = cards_inhand[i][j].value;
            cards_copy_real[i][j].num = cards_inhand[i][j].num;
            cards_copy_real[i][j].picture = cards_inhand[i][j].picture;
            //cards_copy_real[i][j]=*( &cards_inhand[i][j]);
        }
    }
    for(int i=0 ; i<10 ; i++){ //複製一個空手牌
        for(int j=0 ; j<10 ; j++){
            cards_copy[i][j] = cards_inhand[i][j];
            cards_copy[i][j].num = 0;
        }
    }
    cout << "here 1\n";
    result = well_formed(cards_copy_real, cards_copy);
    return result;
}

bool gamer::well_formed(card** cards_inhand, card** cards_copy){
    cout << "here 2\n";
    int total_cards = 0;
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            total_cards += cards_inhand[i][j].num;
            //cout << cards_inhand[i][j].num << '\n';
        }
    }
    cout << total_cards << '\n';

    if(total_cards==2){ //eyes
        cout << "here 3\n";
        print_cards_inhand();
        for(int i=0 ; i<10 ; i++){
            for(int j=0 ; j<10 ; j++){
                cout << "hi ::: " << cards_inhand[i][j].num << '\n';
                if(cards_inhand[i][j].num==2){
                    cout << "here 4\n";
                    return true;
                }
            }
        }
    }

    for(int i=0 ; i<10 ;i++){
        for(int j=0 ; j<10 ; j++){
            if( cards_inhand[i][j].num >= 3 ){
                cards_inhand[i][j].num -= 3;
                cards_copy[i][j].num += 3;
                total_cards -= 3;
                if(well_formed(cards_inhand,cards_copy) == true) return true;
            }
        }
    }
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<8 ; j++){
            if( cards_inhand[i][j].num>=1 && cards_inhand[i][j+1].num>=1 && cards_inhand[i][j+2].num>=1 ){
                cards_inhand[i][j].num--;
                cards_inhand[i][j+1].num--;
                cards_inhand[i][j+2].num--;
                cards_copy[i][j].num++;
                cards_copy[i][j+1].num++;
                cards_copy[i][j+2].num++;
                total_cards -= 3;
                if( well_formed(cards_inhand,cards_copy) ) return true;
            }
        }
    }
    cout << "Hu Return False QQ~\n";
    return false;
}

bool gamer::checkchi(card a){
    cout << "check chi function\n";
    CHI chiconditions[3];

    if(cards_inhand[a.type][a.value-1].num>0 && cards_inhand[a.type][a.value+1].num>0){
        //表示已有ＡC少了B
        chiconditions[0].chiType= a.type;
        chiconditions[0].chiValue[0]=a.value-1;
        chiconditions[0].chiValue[1]=+1;
        canchi=true;
    }
    if(cards_inhand[ a.type ][a.value-2].num>0 && cards_inhand[a.type][a.value-1].num>0){
        //表示有AB少了C
        chiconditions[1].chiType= a.type;
        chiconditions[1].chiValue[0]=a.value-2;
        chiconditions[1].chiValue[1]=a.value-1;
        canchi=true;
    }
    if(cards_inhand[a.type][a.value+1].num>0 && cards_inhand[a.type][a.value+2].num>0){
        //表示有BC少了Ａ
        chiconditions[2].chiType= a.type;
        chiconditions[2].chiValue[0]=a.value+1;
        chiconditions[2].chiValue[1]=a.value+2;
        canchi=true;
    }
    return canchi;
}
bool gamer::checkpeng(card a){
    cout << "checkpeng function\n";
    if(cards_inhand[0][a.value].num==2){
        canpeng=true;
    }
    return canpeng;
}
//以下這邊要同時跳出吃或碰或槓的選項給玩家做選擇
void gamer::printchi(card a){
    LButton chibutton;
    LButton Chi_Card[3][2];
    if(canchi){
        cout<<"canchi"<<endl;
        //SDL的吃選項跳出

        chibutton.LoadFromFile("./mytest/CHI Button.png");
        chibutton.setPosition(400,500);
        chibutton.render(gRenderer );
        SDL_RenderPresent(gRenderer);
        bool chipressed=0;
        while (SDL_PollEvent(&event)==SDL_MOUSEBUTTONUP ||  !chipressed ) {
            chipressed=chibutton.handleEvent();
            if(chipressed){
                haschi=1;
                cout<<"chipressed"<<endl;
                bool chi_presscard=0;
                for(int i=0 ; i<3 ; i++){
                    if(chiconditions[i].chiType!=-1){
                        // 顯示所有chicondition
                        int v1,v2,type;
                        type=chiconditions[i].chiType;
                        v1=chiconditions[i].chiValue[0];
                        v2=chiconditions[i].chiValue[1];
                        string source1,source2;
                        source1= full_pic[type][v1];
                        source2= full_pic[type][v2];
                        Chi_Card[i][0].LoadFromFile(source1);
                        Chi_Card[i][1].LoadFromFile(source2);
                        Chi_Card[i][0].setPosition(300, 420+ i*80);
                        Chi_Card[i][1].setPosition(360, 420+ i*80);
                        Chi_Card[i][0].render(gRenderer);
                        Chi_Card[i][1].render(gRenderer);
                        SDL_RenderPresent(gRenderer);
                        cout << "pysically here.\n";
                    }
                }
                while(SDL_PollEvent(&event)||!chi_presscard){
                    cout << "hiQQQQQ\n";
                    /*if(event.type == SDL_QUIT){
                        running =false;
                    }*/
                    for(int i=0; i<3 && !chi_presscard;i++){
                        for(int j=0; j<2 ; j++ ){
                            chi_presscard = Chi_Card[i][j].handleEvent();
                            if(chi_presscard){
                                loadPage("./mytest/Chi_Page.png");
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1000);
                                int choice=i;
                                HandCards h;
                                int n1,n2,n3;
                                n1=15-Fixed_Card;
                                n2=14-Fixed_Card;
                                n3=13-Fixed_Card;
                                cout << "mentally here.\n";
                                switch(choice){
                                    case 0://執行完吃的動作手中的牌組要刪除吃掉的那一套
                                        cards_inhand[a.type][a.value-1].num-=1;
                                        cards_inhand[a.type][a.value+1].num-=1;
                                        transform_card_to_CSeq(cards_inhand, h);
                                        h.C_Sequence[n1].setCard(a.type,a.value+1);
                                        h.C_Sequence[n2].setCard(a.type,a.value);
                                        h.C_Sequence[n3].setCard(a.type,a.value-1);
                                        Fixed_Card+=3;
                                        h.placeCards();
                                        break;
                                    case 1:
                                        cards_inhand[a.type][a.value-2].num-=1;
                                        cards_inhand[a.type][a.value-1].num-=1;
                                        transform_card_to_CSeq(cards_inhand, h);
                                        h.C_Sequence[n1].setCard(a.type,a.value);
                                        h.C_Sequence[n2].setCard(a.type,a.value-1);
                                        h.C_Sequence[n3].setCard(a.type,a.value-2);
                                        Fixed_Card+=3;
                                        h.placeCards();
                                        break;
                                    case 2:
                                        cards_inhand[a.type][a.value+1].num-=1;
                                        cards_inhand[a.type][a.value+2].num-=1;
                                        transform_card_to_CSeq(cards_inhand, h);
                                        h.C_Sequence[n1].setCard(a.type,a.value+2);
                                        h.C_Sequence[n2].setCard(a.type,a.value+1);
                                        h.C_Sequence[n3].setCard(a.type,a.value);
                                        Fixed_Card+=3;
                                        h.placeCards();
                                        break;
                                }
                                break;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}
void gamer::printpeng(card a){
    LButton pengbutton;
    if(canpeng){
        //SDL的碰選項跳出
        pengbutton.LoadFromFile("./mytest/Pong Button.png");
        pengbutton.setPosition(130, 500);
        pengbutton.render(gRenderer);
        //if player choose to peng
        bool pengpressed=0;
        while (SDL_PollEvent(&event) || !pengpressed){
               pengpressed=pengbutton.handleEvent();
            if(pengpressed){
                haspeng=1;
                loadPage("./mytest/Pong_Page.png");
                SDL_RenderPresent(gRenderer);
                SDL_Delay(1000);
                cards_inhand[a.type][a.value].num-=2;
                HandCards h;
                transform_card_to_CSeq(cards_inhand, h);
                for(int i=0;i<3;i++){
                     h.C_Sequence[15-Fixed_Card-i].setCard(a.type,a.value);
                }
                Fixed_Card+=3;
                h.placeCards();
            }
        }
    }
}


class player : public gamer{
private:
   const int max_card_num;

public:
    player();  //初始化手牌
    //player(const player&); //copy constructor
    ~player(); // destructor

    player& operator++(); //抽牌
    player operator!();
    void draw_card(); //抽牌
    card play(int); //回合出牌
    int get_CARD_NUMBER();
    int get_max_card_num();
    //void sort(); //理牌  //按照順序顯示牌
};

player::player():max_card_num(4){
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            cards_inhand[i][j].type = static_cast<suit>(i);  //bugs???
            cards_inhand[i][j].value = j;
            cards_inhand[i][j].num = 0;
            cards_inhand[i][j].picture = pic[i][j];
        }
    }
}
player::~player(){
    for(int i=0 ; i<10 ; i++){
        delete [] cards_inhand[i];
    }
    delete [] cards_inhand;
}

int player::get_CARD_NUMBER(){
    return NUMBER_OF_CARDS;
}
int player::get_max_card_num(){
    return max_card_num;
}

/*void player::sort(){
    HandCards Players_Cards;
    transform_card_to_CSeq(cards_inhand,Players_Cards);
    Players_Cards.placeCards();
}*/

card player::play(int i){
    //玩家用游標點牌
    HandCards h;
    transform_card_to_CSeq(cards_inhand, h);
    bool presscard[17];
    suit t;
    int v=0;
    for(int i=0;i<17;i++){
        presscard[i]= h.C_Sequence[i].handleEvent();
        if(presscard[i]){
            v = h.C_Sequence[i].getValue();
            t = h.C_Sequence[i].getType();
            cards_inhand[static_cast<suit>(t)][v].num--;
            //出牌後 直接成為圖片放桌上 理牌的時候就會瞬間消失
        }
    }
    return cards_inhand[static_cast<suit>(t)][v];
}

void player::draw_card(){
    card tem;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 33);
    int ran=0;
    for(int i=0 ; i<1 ; i++){
        ran = dis(gen);
        if(ran<27){      //一般的牌
            int a = int(ran/9);   //Tong or Line or Wan
            int b = int(ran%9)+1;   //value
            //cout << "(a , b) = " << '(' << a <<" , "<< b << ")\n";
            if(Remain_cards[a][b].num==0 or cards_inhand[a][b].num==3){
                i--;
            }
            else{
                tem.value=cards_inhand[a][b].value;
                tem.type=cards_inhand[a][b].type;
                cards_inhand[a][b].num++;
                Remain_cards[a][b].num--;
            }
        }
        else{            //風中發白  //ran=27~33
            int c = ran-24;
            //cout << "c = " << c << '\n';
            if(Remain_cards[c][0].num == 0 or cards_inhand[c][0].num == 3){
                i--;
            }
            else{
                //tem.num=cards_inhand[c][0].num;
                tem.value=cards_inhand[c][0].value;
                tem.type=cards_inhand[c][0].type;
                cards_inhand[c][0].num++;
                Remain_cards[c][0].num--;
            }
        }
    }
    //cout << "down here!\n";
    //顯示新抽到的牌
    aCard newin;
    cout << tem.type << ' ' << tem.value << '\n';
    newin.setCard(tem.type,tem.value);
    newin.setPosition(610, 515); //放中間上面
    newin.render(gRenderer);

}

player& player::operator++(){
    //cout << "in~~\n";
    draw_card();
    //cout << "drawed\n";
    return *this;
}

player player::operator!(){
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            cards_inhand[i][j].type = static_cast<suit>(i);
            cards_inhand[i][j].value = j;
            cards_inhand[i][j].num = 0;
            cards_inhand[i][j].picture = pic[i][j];
        }
    }
    return *this;
}

//computer!!!
class computer : public gamer{
private:
    static int  max_card_num;

public:
    computer();
    computer(const computer &r);
    ~computer();
    //void clear();
    //card last; //最後拿的牌的資料
    card play(); //出一張牌
    void draw_card();
    bool check_right_number(card ** cards_inhand);
    computer & operator++();
    computer operator!();
    /*void printchi(card a);//print out all chi choices
    void printgang(card a);//print out all gang choices
    void printpeng(card a);//print out all peng choices*/
};
int computer::max_card_num=4;
computer::computer(){  //初始化
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            cards_inhand[i][j].type = static_cast<suit>(i);  //bugs???
            cards_inhand[i][j].value = j;
            cards_inhand[i][j].num = 0;
        }
    }
}
computer::computer(const computer &c){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cards_inhand[i][j].num=c.cards_inhand[i][j].num;
            cards_inhand[i][j].type=c.cards_inhand[i][j].type;
            cards_inhand[i][j].value=c.cards_inhand[i][j].value;
        }
    }
    for(int i=0;i<3;i++){
        chiconditions[i].chiType=c.chiconditions[i].chiType;
        chiconditions[i].chiValue[0]=c.chiconditions[i].chiValue[0];
        chiconditions[i].chiValue[1]=c.chiconditions[i].chiValue[1];
    }
}
computer& computer::operator++(){
    draw_card();
    return *this;
}
computer computer::operator!(){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cards_inhand[i][j].num=0;
            cards_inhand[i][j].value=0;
            cards_inhand[i][j].type=NONE;
        }
    }
    return *this;
}
computer::~computer(){
    for(int i=0 ; i<10 ; i++){
        delete [] cards_inhand[i];
    }
    delete [] cards_inhand;
}

card computer::play(){
    // 1. 前後0 自己1
    card card_now;
    for(int i=0 ; i<10 ; i++){  //先看1
        if(cards_inhand[i][0].num == 1 && cards_inhand[i][1].num == 0){
            //丟掉它！！！
            card_now = cards_inhand[i][0];
            cards_inhand[i][0].num--;
            return card_now;
        }
    }
    for(int i=0 ; i<10 ; i++){  //再看9
        if(cards_inhand[i][9].num == 1 && cards_inhand[i][8].num == 0){
            //丟掉它！！！
            card_now = cards_inhand[i][9];
            cards_inhand[i][9].num--;
            return card_now;
        }
    }
    for(int i=0 ; i<10 ; i++){   //看其他
        for(int j=1 ; j<9 ; j++){
            if(cards_inhand[i][j].num == 1 && cards_inhand[i][j+1].num == 0 && cards_inhand[i][j-1].num == 0){
                card_now = cards_inhand[i][j];
                cards_inhand[i][j].num--;
                return card_now;
            }
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 33);
    int ran=0;
    for(int i=0 ; i<1 ; i++){
        ran = dis(gen)%34;
        if(ran<27){      //一般的牌
            int a = int(ran/9);   //Tong or Line or Wan
            int b = int(ran%9)+1;   //value
            if(cards_inhand[a][b].num == 0){
                i--;
            }
            else{
                cards_inhand[a][b].num--;
                card_now = cards_inhand[a][b];
                return card_now;
            }
        }
        else{            //風中發白  //ran=27~33
            int c = ran-24;
            if(cards_inhand[c][0].num == 0){
                i--;
            }
            else{
                cards_inhand[c][0].num--;
                card_now = cards_inhand[c][0];

            }
        }
    }
    return card_now;
}
bool computer::check_right_number(card ** cards_inhand){
    int  tnum=0;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            tnum = cards_inhand[i][j].num;
            if(tnum>max_card_num)return false;
        }
    }
    return true;
}

void computer::draw_card(){
    card tem;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 33);
    int ran=0;
    for(int i=0 ; i<1 ; i++){
        ran = dis(gen);
        if(ran<27){      //一般的牌
            int a = int(ran/9);   //Tong or Line or Wan
            int b = int(ran%9)+1;   //value
            //cout << "(a , b) = " << '(' << a <<" , "<< b << ")\n";
            if(Remain_cards[a][b].num==0 or cards_inhand[a][b].num>=3){
                i--;
            }
            else{
                tem=cards_inhand[a][b];
                cards_inhand[a][b].num++;
                Remain_cards[a][b].num--;
            }
        }
        else{            //風中發白  //ran=27~33
            int c = ran-24;
            //cout << "c = " << c << '\n';
            if(Remain_cards[c][0].num == 0 or cards_inhand[c][0].num >= 3){
                i--;
            }
            else{
                tem=cards_inhand[c][0];
                cards_inhand[c][0].num++;
                Remain_cards[c][0].num--;
            }
        }
    }
}

void transform_card_to_CSeq(card** cardsInHand,HandCards &players){
    int num=0;
    for(int i=0 ; i<10 ; i++){
        for(int j=0 ; j<10 ; j++){
            for(int k=0 ; k<cardsInHand[i][j].num ; k++){
                suit t = cardsInHand[i][j].type;
                int v = cardsInHand[i][j].value;
                players.C_Sequence[num].setCard(t , v);
                num++;
            }
        }
    }
}

void transform_Cseq_to_Card(card** cardsInHand, HandCards &Hand){
    for(int i=0 ; i<16 ; i++){
        int t = Hand.C_Sequence[i].getType();
        int v = Hand.C_Sequence[i].getValue();
        cardsInHand[t][v].type = static_cast<suit>(t);
        cardsInHand[t][v].value = v;
    }
}



//main function begins

int main(int argc, char *argv[]){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if ((IMG_Init(flags) & flags) != flags) {
        std::cout << "IMG_Init: Failed to init required jpg and png support!\n";
        std::cout << "IMG_Init: " << IMG_GetError() << std::endl;
        // handle error
    }
    gWindow = SDL_CreateWindow("Hello World", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!gWindow) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gWindow: %s", SDL_GetError());
        return 3;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gRenderer: %s", SDL_GetError());
        return 3;
    }
    gSurface = IMG_Load("./mytest/starting_page.png");
    if (!gSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gSurface from image: %s", SDL_GetError());
        return 3;
    }
    texture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from gSurface: %s", SDL_GetError());
        return 3;
    }
    int mflags = MIX_INIT_MP3;
    if ((Mix_Init(mflags) & mflags) != mflags) {
        std::cout << "Mix_Init: Failed to init required ogg and mod support!\n";
        std::cout << "Mix_Init: %s\n" << Mix_GetError() << std::endl;
        return 3;
    }

    if (Mix_OpenAudio(128000, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cout << "Mix_OpenAudio: %s\n" << Mix_GetError() << std::endl;
        return 3;
    }
    Mix_Music *music;
    music = Mix_LoadMUS("./mp3/Guitar House - josh pan.mp3");
    if (!music) {
        std::cout << "Mix_LoadMUS(\"Guitar House - josh pan.mp3\"): %s\n" << Mix_GetError();
        return 3;
    }
    Mix_PlayMusic(music, -1);
    SDL_FreeSurface(gSurface);

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, texture, NULL, NULL);
    SDL_RenderPresent(gRenderer);

///////////////////////////////////////////////////////////////////////////////
    ///////點擊xinf -> 進入主頁
    //start
    bool running = true;
    bool pages[4] = {0, 0, 0, 0};
    card card_now;  //where 2 place??
    while (running) {

        for(int i=0 ; i<10 ; i++){        //initialize remain
            for(int j=0 ; j<10 ; j++){
                Remain_cards[i][j].type = static_cast<suit>(i);
                Remain_cards[i][j].value = j;
                Remain_cards[i][j].num = 4;
            }
        }
        player user;
        computer comArr[3];


        SDL_PollEvent(&event);
        //end
        if (event.type == SDL_QUIT){
            running = false;
        }
        //點擊畫面開始
        if (event.type == SDL_MOUSEBUTTONUP && !pages[0]) {
            pages[0] = true;
            loadPage("./mytest/char page_4.png");
            event.type = SDL_MOUSEMOTION;

            LButton starter;
            starter.LoadFromFile("./mytest/500start.png");
            starter.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT*2/3);
            starter.render(gRenderer);
            //SDL_RenderPresent(gRenderer);

            LButton Char[5];
            Char[0].LoadFromFile("./mytest/Char-1.png");
            Char[1].LoadFromFile("./mytest/Char-2.png");
            Char[2].LoadFromFile("./mytest/Char-3.png");
            Char[3].LoadFromFile("./mytest/Char-4.png");
            Char[4].LoadFromFile("./mytest/Char-5.png");
            for (int i = 0; i < 5; ++i) {
                Char[i].setPosition(SCREEN_WIDTH/2-480+240*i, 200);
                Char[i].render(gRenderer);
            }
            SDL_RenderPresent(gRenderer);


            bool Pressed = false;
            while ((SDL_PollEvent(&event) || !Pressed) && !pages[1]) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    //cout << "quit\n";
                }
                Pressed = starter.handleEvent();
                if (Pressed && !pages[1]) {
                    loadPage("./mytest/2nd page_3.png");
                    //cout << "load page\n";
                    pages[1] = true;
                    break;
                }
            }

            // main page
            bool Pressed2 = 0;
            LButton starter2;
            starter2.LoadFromFile("./mytest/mahjong.png");
            starter2.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 2 / 3);
            starter2.render(gRenderer);
            //SDL_RenderPresent(gRenderer);
            HandCards Players_Cards;

            // deal_cards
            // user deal
            user.deal();
            //確認牌數正確
            int total_cards = 0;
            for(int i=0 ; i<10 ; i++){
                for(int j=0 ; j<10 ; j++){
                    total_cards += user.cards_inhand[i][j].num;
                }
            }
            if(total_cards != user.get_CARD_NUMBER()){
                cout << "user's handcards number error.\n";
            }

            transform_card_to_CSeq(user.cards_inhand,Players_Cards);
            Players_Cards.placeCards();

            // computer deal
            for(int i=0 ; i<3 ; i++){
                comArr[i].deal();
            }

            // start playing
/*
            //test begin
            !user;
            for(int i=2 ; i<7 ; i++){
                user.cards_inhand[0][i].num+=3;
            }
            
            user.cards_inhand[0][1].num++;
            user.cards_inhand[0][1].num++;
            user.print_cards_inhand();
            if(user.checkHu()){
                cout << "HU!!!!!\n";
            }
            else cout << "no hu~\n";
            // test end
*/
            // user draw card
            ++user;
            
            SDL_RenderPresent(gRenderer);
            cout << "render\n";
            //SDL_Delay(4000);


//////////////////////////////////////////////////////////////////
            Pressed2 = false;
            int counter = 0;
            aCard the_out_card;
            int current_player=0; //0:player , 1/2/3:com1/2/3
            bool end=0;
            bool win=0;
            //bool haschi=0;
            bool keep_checking = 1;
            user.chi_or_peng = 1;

            while(!end){
                cout<<"in while not end\n";
                cout<<"current player="<<current_player<<endl;
                if(current_player==0){   // //0:player , 1/2/3:com0/1/2
                    cout<<"THE USER player Round.\n";
                    keep_checking = 1;
                    if(user.chi_or_peng==0){
                        ++user; //抽一張牌
                        SDL_RenderPresent(gRenderer);
                    }
                    //user.print_cards_inhand();
                    //cout << "----------\n";
                    bool a = user.checkHu();
                    //user.print_cards_inhand();
                    //cout << "----------\n";
                    //cout << "a = " << a << '\n';
                    user.chi_or_peng=0;
                    if(a){   // check if user hu or not
                        cout << "hubutton\n";
                        LButton hubutton;
                        hubutton.LoadFromFile("./mytest/HOO Button.png");
                        hubutton.setPosition(310, 500);
                        hubutton.render(gRenderer);
                        SDL_RenderPresent(gRenderer);
                        while(!hubutton.handleEvent() || SDL_PollEvent(&event)){
                            cout << "uueueueu\n";
                            loadPage("./mytest/Hoo_Page_Y.png");
                            SDL_RenderPresent(gRenderer);
                            win = 1;
                            end = 1;
                            break;
                        }
                    }

                    counter=0;
                    while(SDL_PollEvent(&event) || counter < 1) {  //出牌
                        //cout << "in!\n";
                        if (event.type == SDL_QUIT) {
                            running = false;
                        }
                        Pressed2 = false;
                        for (int i = 0; i < 17 && !Pressed2; ++i){
                            Pressed2 = Players_Cards.C_Sequence[i].handleEvent();
                            if (Pressed2) {
                                cout << "pressed2!\n";
                                the_out_card = Players_Cards.C_Sequence[i];
                                the_out_card.go_Board();
                                Players_Cards.placeCards();
                                SDL_RenderPresent(gRenderer);
                                cout<< "goboard\n";

                                user.cards_inhand[the_out_card.getType()][the_out_card.getValue()].num--;
                                card_now.type = the_out_card.getType();
                                card_now.value = the_out_card.getValue();
                                int total_cards;
                                total_cards = 0;
                                /*for(int k=0 ; k<10 ; k++){   //印出全部ㄉ牌 用來debug
                                    for(int j=0 ; j<10 ; j++){
                                        total_cards += user.cards_inhand[k][j].num;
                                    }
                                }
                                cout << "total = " << total_cards << '\n';
                                user.print_cards_inhand();*/
                                transform_card_to_CSeq(user.cards_inhand, Players_Cards);
                                /*cout << "<<<<<******>>>>>\n";
                                for(int h=0 ; h<16 ; h++){
                                    cout << Players_Cards.C_Sequence[h].getType() << '\n';
                                }*/
                                Players_Cards.placeCards();
                                SDL_RenderPresent(gRenderer);
                                counter++;
                                cout << "break\n";
                                break;
                            }
                        }
                    }
                    cout << "check hu\n";
                    //判斷其他人有沒有功能
                    for(int j=0 ; j<3 ; j++){        // hu
                        if(comArr[j].checkHu(card_now)){
                            cout << "Player " << j << " Hu!!!\n";
                            loadPage("./mytest/loser_page.png");
                            SDL_RenderPresent(gRenderer);
                            end = 1;
                            keep_checking = 0;
                            break;
                        }
                        else{
                            cout << "safe\n";
                        }
                    }

                    // 碰
                    cout << "check peng\n";
                    if(keep_checking){
                        cout << "in!!\n";
                        aCard a,b;
                        a.setCard(the_out_card.getType() , the_out_card.getValue());
                        b.setCard(the_out_card.getType() , the_out_card.getValue());
                        cout << comArr[0].checkpeng(card_now) << '\n';
                        if(comArr[0].checkpeng(card_now)){
                            cout << "comArr[0] Peng!\n";
                            the_out_card.load_sets_1(a,b);
                            current_player = 1;
                            comArr[0].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[0].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        else if(comArr[1].checkpeng(card_now)){
                            cout << "comArr[1] Peng!\n";
                            the_out_card.load_sets_2(a,b);
                            current_player = 2;
                            comArr[1].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[1].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        else if(comArr[2].checkpeng(card_now)){
                            cout << "comArr[2] Peng!\n";
                            the_out_card.load_sets_3(a,b);
                            current_player = 3;
                            comArr[2].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[2].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        cout << "check peng end yaya ^_^\n";
                    }

                    //吃
                    cout << "check chi\n";
                    if(keep_checking){
                        cout<<"in 0 Chi!"<<endl;
                        if(comArr[0].checkchi(card_now)){
                            cout << "chi-ed!!\n";
                            comArr[0].chi_or_peng = 1;
                            current_player = 1;
                            keep_checking = 0;
                            for(int i=0 ; i<3;i++){
                                if(comArr[0].chiconditions[i].chiType!=-1){

                                    aCard a,b;
                                    int v1,v2;
                                    v1 = comArr[0].chiconditions[i].chiValue[0];
                                    v2 = comArr[0].chiconditions[i].chiValue[1];
                                    a.setCard(card_now.type,v1);
                                    b.setCard(card_now.type,v2);
                                    comArr[0].cards_inhand[the_out_card.getType()][v1].num -= 1;
                                    comArr[0].cards_inhand[card_now.type][v2].num -=1 ;
                                    the_out_card.load_sets_1(a, b);
                                    break;
                                }
                            }
                        }
                        else cout<<"nono"<<endl;
                    }
                    if(current_player==0){
                        current_player++;
                    }
                }

                if(current_player==1){   // //0:player , 1/2/3:com0/1/2
                    cout<<"THE 1st computer player Round.\n";
                    keep_checking = 1;
                    if(comArr[0].chi_or_peng==0){
                        ++comArr[0];//抽一張牌
                    }
                    bool a = comArr[0].checkHu();
                    //comArr[0].print_cards_inhand();
                    comArr[0].chi_or_peng=0;
                    if(a){   // check if comArr[0] hu or not
                        /*cout << "hubutton\n";
                        LButton hubutton;
                        hubutton.LoadFromFile("./mytest/HOO Button.png");
                        hubutton.setPosition(310, 500);
                        hubutton.render(gRenderer);
                        SDL_RenderPresent(gRenderer);*/
                        //while(!hubutton.handleEvent() || SDL_PollEvent(&event)){
                        cout << "comArr[0]HU~~~\n";
                        loadPage("./mytest/loser_page.png");
                        SDL_RenderPresent(gRenderer);
                        win = 0;
                        end = 1;
                        //}
                    }

                    counter=0;
                    card_now = comArr[0].play(); //電腦出牌
                    the_out_card.setCard(card_now.type , card_now.value);
                    the_out_card.go_Board();
                    Players_Cards.placeCards();
                    SDL_RenderPresent(gRenderer);

                    //先看user有沒有胡
                    user.chi_or_peng=0;
                    if(user.checkHu(card_now)){   // if user hu
                        cout << "hubutton\n";
                        LButton hubutton;
                        hubutton.LoadFromFile("./mytest/HOO Button.png");
                        hubutton.setPosition(310, 500);
                        hubutton.render(gRenderer);
                        SDL_RenderPresent(gRenderer);
                        while(!hubutton.handleEvent() || SDL_PollEvent(&event)){
                            cout << "uueueueu\n";
                            loadPage("./mytest/Hoo_Page_Y.png");
                            SDL_RenderPresent(gRenderer);
                            win = 1;
                            end = 1;
                            break;
                        }
                    }
                    //判斷其他人有沒有功能
                    for(int j=1 ; j<3 ; j++){        // hu
                        if(comArr[j].checkHu(card_now)){
                            loadPage("./mytest/loser_page.png");
                            SDL_RenderPresent(gRenderer);
                            end = 1;
                            keep_checking = 0;
                            break;
                        }
                        else{
                            cout << "safe\n";
                        }
                    }

                    // 碰
                    // user peng
                    cout << "check peng\n";
                    if(keep_checking){
                        cout << "check if user peng\n";
                        if(user.checkpeng(card_now)){
                            //碰的動作
                            user.printpeng(card_now);
                        }
                    }
                    // computer peng
                    if(keep_checking){
                        cout << "in!!\n";
                        aCard a,b;
                        a.setCard(the_out_card.getType() , the_out_card.getValue());
                        b.setCard(the_out_card.getType() , the_out_card.getValue());
                        //cout << comArr[0].checkpeng(card_now) << '\n';
                        if(comArr[1].checkpeng(card_now)){
                            the_out_card.load_sets_2(a,b);
                            current_player = 2;
                            comArr[1].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[1].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        else if(comArr[2].checkpeng(card_now)){
                            the_out_card.load_sets_3(a,b);
                            current_player = 3;
                            comArr[2].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[2].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        cout << "check peng2 end yaya ^_^\n";
                    }

                    //吃
                    cout << "check chi\n";
                    if(keep_checking){
                        cout<<"in 1 Chi!"<<endl;
                        if(comArr[1].checkchi(card_now)){
                            comArr[1].chi_or_peng = 1;
                            //current_player = 2;
                            keep_checking = 0;
                            for(int i=0 ; i<3;i++){
                                if(comArr[1].chiconditions[i].chiType!=-1){
                                    aCard a,b;
                                    int v1,v2;
                                    v1 = comArr[1].chiconditions[i].chiValue[0];
                                    v2 = comArr[1].chiconditions[i].chiValue[1];
                                    a.setCard(card_now.type,v1);
                                    b.setCard(card_now.type,v2);
                                    comArr[1].cards_inhand[the_out_card.getType()][v1].num -= 1;
                                    comArr[1].cards_inhand[card_now.type][v2].num -=1 ;
                                    the_out_card.load_sets_1(a, b);
                                    break;
                                }
                            }
                        }
                        else cout<<"nono"<<endl;
                    }
                    if(current_player==1){
                        current_player++;
                    }
                }

                if(current_player==2){   // //0:player , 1/2/3:com0/1/2
                    cout<<"THE 2rd computer player Round.\n";
                    keep_checking = 1;
                    if(comArr[1].chi_or_peng==0){
                        ++comArr[1]; //抽一張牌
                    }
                    //comArr[1].print_cards_inhand();
                    comArr[1].chi_or_peng=0;
                    if( comArr[1].checkHu() ){   // check if comArr[1] hu or not
                        cout << "comArr[1]HU~~~\n";
                        loadPage("./mytest/loser_page.png");
                        SDL_RenderPresent(gRenderer);
                        win = 0;
                        end = 1;
                    }

                    counter=0;
                    card_now = comArr[1].play(); //電腦2出牌
                    the_out_card.setCard(card_now.type , card_now.value);
                    the_out_card.go_Board();
                    Players_Cards.placeCards();
                    SDL_RenderPresent(gRenderer);

                    //先看user有沒有胡
                    user.chi_or_peng=0;
                    if(user.checkHu(card_now)){   // if user hu
                        cout << "hubutton\n";
                        LButton hubutton;
                        hubutton.LoadFromFile("./mytest/HOO Button.png");
                        hubutton.setPosition(310, 500);
                        hubutton.render(gRenderer);
                        SDL_RenderPresent(gRenderer);
                        while(!hubutton.handleEvent() || SDL_PollEvent(&event)){
                            cout << "uueueueu\n";
                            loadPage("./mytest/Hoo_Page_Y.png");
                            SDL_RenderPresent(gRenderer);
                            win = 1;
                            end = 1;
                            break;
                        }
                    }
                    //判斷其他人有沒有功能
                    for(int j=0 ; j<3 ; j+=2){        // hu
                        if(comArr[j].checkHu(card_now)){
                            loadPage("./mytest/loser_page.png");
                            SDL_RenderPresent(gRenderer);
                            end = 1;
                            keep_checking = 0;
                            break;
                        }
                        else{
                            cout << "safe\n";
                        }
                    }

                    // 碰
                    // user peng
                    cout << "check peng\n";
                    if(keep_checking){
                        cout << "check if user peng\n";
                        if(user.checkpeng(card_now)){
                            //碰的動作
                            user.printpeng(card_now);
                        }
                    }
                    // computer peng
                    if(keep_checking){
                        aCard a,b;
                        a.setCard(the_out_card.getType() , the_out_card.getValue());
                        b.setCard(the_out_card.getType() , the_out_card.getValue());
                        if(comArr[0].checkpeng(card_now)){
                            the_out_card.load_sets_1(a,b);
                            current_player = 1;
                            comArr[0].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[0].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        else if(comArr[2].checkpeng(card_now)){
                            the_out_card.load_sets_3(a,b);
                            current_player = 3;
                            comArr[2].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[2].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        cout << "check peng3 end yaya ^_^\n";
                    }

                    //吃
                    cout << "check chi\n";
                    if(keep_checking){
                        cout<<"in 2 Chi!"<<endl;
                        if(comArr[2].checkchi(card_now)){
                            comArr[2].chi_or_peng = 1;
                            //current_player = 3;
                            keep_checking = 0;
                            for(int i=0 ; i<3;i++){
                                if(comArr[2].chiconditions[i].chiType!=-1){
                                    aCard a,b;
                                    int v1,v2;
                                    v1 = comArr[2].chiconditions[i].chiValue[0];
                                    v2 = comArr[2].chiconditions[i].chiValue[1];
                                    a.setCard(card_now.type,v1);
                                    b.setCard(card_now.type,v2);
                                    comArr[2].cards_inhand[the_out_card.getType()][v1].num -= 1;
                                    comArr[2].cards_inhand[card_now.type][v2].num -=1 ;
                                    the_out_card.load_sets_1(a, b);
                                    break;
                                }
                            }
                        }
                        else cout<<"nono"<<endl;
                    }
                    if(current_player==2){
                        current_player++;
                    }
                }

                if(current_player==3){   // //0:player , 1/2/3:com0/1/2
                    cout<<"THE 3rd computer player Round.\n";
                    keep_checking = 1;
                    if(comArr[2].chi_or_peng==0){
                        ++comArr[2]; //抽一張牌
                    }
                    //comArr[1].print_cards_inhand();
                    comArr[2].chi_or_peng=0;
                    if( comArr[2].checkHu() ){   // check if comArr[2] hu or not
                        cout << "comArr[2]HU~~~\n";
                        loadPage("./mytest/loser_page.png");
                        SDL_RenderPresent(gRenderer);
                        win = 0;
                        end = 1;
                    }

                    counter=0;
                    card_now = comArr[2].play(); //電腦出牌
                    the_out_card.setCard(card_now.type , card_now.value);
                    the_out_card.go_Board();
                    Players_Cards.placeCards();
                    SDL_RenderPresent(gRenderer);

                    //先看user有沒有胡
                    user.chi_or_peng=0;
                    if(user.checkHu(card_now)){   // if user hu
                        cout << "hubutton\n";
                        LButton hubutton;
                        hubutton.LoadFromFile("./mytest/HOO Button.png");
                        hubutton.setPosition(310, 500);
                        hubutton.render(gRenderer);
                        SDL_RenderPresent(gRenderer);
                        while(!hubutton.handleEvent() || SDL_PollEvent(&event)){
                            cout << "uueueueu\n";
                            loadPage("./mytest/Hoo_Page_Y.png");
                            SDL_RenderPresent(gRenderer);
                            win = 1;
                            end = 1;
                            break;
                        }
                    }
                    //判斷其他人有沒有功能
                    for(int j=0 ; j<2 ; j++){        // hu
                        if(comArr[j].checkHu(card_now)){
                            loadPage("./mytest/loser_page.png");
                            SDL_RenderPresent(gRenderer);
                            end = 1;
                            keep_checking = 0;
                            break;
                        }
                        else{
                            cout << "safe\n";
                        }
                    }

                    // 碰
                    // user peng
                    cout << "check peng\n";
                    if(keep_checking){
                        cout << "check if user peng\n";
                        if(user.checkpeng(card_now)){
                            //碰的動作
                            user.printpeng(card_now);
                        }
                    }
                    // computer peng
                    if(keep_checking){
                        aCard a,b;
                        a.setCard(the_out_card.getType() , the_out_card.getValue());
                        b.setCard(the_out_card.getType() , the_out_card.getValue());
                        if(comArr[0].checkpeng(card_now)){
                            the_out_card.load_sets_1(a,b);
                            current_player = 1;
                            comArr[0].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[0].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        else if(comArr[1].checkpeng(card_now)){
                            the_out_card.load_sets_2(a,b);
                            current_player = 2;
                            comArr[1].chi_or_peng = 1;
                            keep_checking = 0;
                            comArr[1].cards_inhand[the_out_card.getType()][the_out_card.getValue()].num -= 2;
                        }
                        cout << "check peng3 end yaya ^_^\n";
                    }

                    //吃
                    cout << "check chi\n";
                    if(keep_checking){
                        cout<<"in 3 Chi!"<<endl;
                        if(user.checkchi(card_now)){
                            user.chi_or_peng=1;
                            keep_checking=0;
                            current_player=0;
                            user.printchi(card_now);
                        }
                        else cout<<"nono chi"<<endl;
                    }
                    if(current_player==3){
                        current_player=0;
                    }
                }
            }// while後面ㄉ括號

            if(win)
            {
                loadPage("./mytest/Hoo_Page_Y.png");
            } else{
                loadPage("./mytest/loser_page.png");
                //loadTexture("./mytest/loser_page.png",SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
            }


            LButton restart;
            restart.LoadFromFile("./mytest/Re-start.png");
            cout<<"reStart pos set!\n";
            restart.setPosition(4*SCREEN_WIDTH/5,1*SCREEN_HEIGHT/2);
            restart.render(gRenderer);
            //SDL_RenderPresent(gRenderer);

            LButton quit;
            quit.LoadFromFile("./mytest/Quit.png");
            cout<<"quit pos set!\n";
            quit.setPosition(4*SCREEN_WIDTH/5,1*SCREEN_HEIGHT/2+120);
            quit.render(gRenderer);
            SDL_RenderPresent(gRenderer);

            bool end_pose = false;
            while (!end_pose) {
                SDL_PollEvent(&event);
                if (event.type == SDL_QUIT||(quit.handleEvent())) {
                    cout << "quit\n";
                    end_pose=true;
                    running = false;
                    break;
                }

                else if (restart.handleEvent()) {
                    end_pose=true;
                    Board_num = 0;//桌上的第幾張牌
                    first_num = 0;//桌上的第幾張牌
                    second_num = 0;//桌上的第幾張牌
                    third_num = 0;//桌上的第幾張牌
                    break;
                }
            }


        }
        //SDL_RenderPresent(gRenderer);
    }
    //刪除全部的東東
    close();
    return 0;
}
