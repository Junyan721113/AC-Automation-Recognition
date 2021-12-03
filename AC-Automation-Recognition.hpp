// AC-Automation-Recognition.hpp: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#include <vector>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <array>

class AC_Automation
{
public:
    AC_Automation() {
        Trie_Tree.resize(1);
        isWordEnd.resize(1);
        fail.resize(1);
        //Trie_Tree[0].resize(128);
    }
    std::queue <int> q;
    int Tree_Size = 0;
    std::vector <std::array <int, 128>> Trie_Tree;
    std::vector <int> isWordEnd;
    std::vector <int> fail;

    struct List_Data {
        std::string Word;
        int WordCount = 0;
    };

    struct Result_Data {
        std::string Word;
        int MatchPosition;
    };

    std::vector <List_Data> Word_List;
    std::vector <Result_Data> Auto_Result;

    enum Read_Mode {
        ReadFileMode = 0,
        ReadConsoleMode = 1
    };

    void readwords(std::string in_file, Read_Mode Mode) {
        FILE* inFileStream;
        if (Mode == ReadConsoleMode) inFileStream = _popen(in_file.c_str(), "r");
        else inFileStream = fopen(in_file.c_str(), "r");
        printf("Loading keywords in %s...", &in_file[0]);
        char* p;
        Word_List.push_back(List_Data{ "", 0 });
        while (fscanf(inFileStream, "%[^\n]%*c", p) != EOF) ins(std::string(p, strlen(p)));//printf("%s\n", &p[0]), 
        printf(" Done!\n");
        printf("Building keywords tree...");
        build();
        printf(" Done!\n");
    }

    void ins(std::string s) {
        //printf(s.c_str());
        int len = s.length();
        int now = 0;
        //printf("%d ", len);
        for (int i = 0; i < len; i++) {
            int v = s[i];
            //printf("%d %d ", Trie_Tree[now][v], i);
            //printf(s);
            if (!Trie_Tree[now][v]) {
                Trie_Tree[now][v] = ++Tree_Size;
                Trie_Tree.resize(Tree_Size + 1);
                isWordEnd.resize(Tree_Size + 1);
                fail.resize(Tree_Size + 1);
                /*Trie_Tree.push_back({0});
                isWordEnd.push_back(0);
                fail.push_back(0);*/
                //Trie_Tree[Tree_Size].resize(128);
                //for(int j = 0; j < 128; j++) Trie_Tree[Tree_Size][j] = 0;
            }
            //printf("\n");
            now = Trie_Tree[now][v];
        }
        isWordEnd[now] = Word_List.size();
        Word_List.push_back(List_Data{ s, 0 });
        //printf("\n%d: %d\n", now, WordList_Size);
    }

    void build() {
        for (int i = 0; i < 128; i++) if (Trie_Tree[0][i]) fail[Trie_Tree[0][i]] = 0, q.push(Trie_Tree[0][i]);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < 128; i++)
                if (Trie_Tree[u][i]) fail[Trie_Tree[u][i]] = Trie_Tree[fail[u]][i], q.push(Trie_Tree[u][i]);
                else Trie_Tree[u][i] = Trie_Tree[fail[u]][i];
        }
        //printf("##############################");
    }

    void query(std::string in_file, std::string out_file, Read_Mode Mode) {
        printf("Extracting keywords...");
        FILE* inFileStream;
        if (Mode == ReadConsoleMode) inFileStream = _popen(in_file.c_str(), "r");
        else inFileStream = fopen(in_file.c_str(), "r");
        FILE* outFileStream = fopen(out_file.c_str(), "w");
        //freopen(o, "w", stdout);
        int now = 0, ans = 0, FilePos = -1;
        char ch;
        while (fscanf(inFileStream, "%c", &ch) != EOF) {
            //printf("%d ", now);
            FilePos++;
            int v = ch;
            if (v < 0 || v >= 128) continue;
            //printf("%c", ch);
            now = Trie_Tree[now][v];
            if (isWordEnd[now]) {
                Word_List[isWordEnd[now]].WordCount++;
                Auto_Result.push_back(Result_Data{ Word_List[isWordEnd[now]].Word, FilePos });
            }
            //printf("\n#\n");
        }

        fprintf(outFileStream, "With %d keywords ", int(Word_List.size()));
        for (int i = 0; i < Word_List.size(); i++) fprintf(outFileStream, "\"%s\"(%d), ", Word_List[i].Word.c_str(), Word_List[i].WordCount);
        fprintf(outFileStream, "the tree size: %d, %d, %d\n\n", int(Trie_Tree.size()), int(isWordEnd.size()), int(fail.size()));
        fprintf(outFileStream, "Get %d results in total:\n\n", int(Auto_Result.size()));
        for (int i = 0; i < Auto_Result.size(); i++) fprintf(outFileStream, "At %d get:\n%s\n\n", Auto_Result[i].MatchPosition, Auto_Result[i].Word.c_str());

        fclose(inFileStream);
        fclose(outFileStream);
        printf(" Done!\n");
        return;
    }
};

// TODO: 在此处引用程序需要的其他标头。
