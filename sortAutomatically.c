#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 255
#define DIC_SIZE 100

int word_count = 0; // 다음에 단어가 저장될 위치 & 저장된 단어의 갯수

typedef struct word_dic {
    char* word;
    int line_num[15];
} word_dic;

char* exc[20] = { "a", "the", "when", "where", "why", "who", "is", "are", "which", "how",
                    "to", "for", "in", "at", "of", "on", "by", "but", "so", "and" };


char* Word_make(char* ptr)
{
    if (ptr[strlen(ptr) - 1] == '\n') // \n인 경우에는 포함시키지 않는다.
        ptr[strlen(ptr) - 1] = '\0';
    if (ptr[strlen(ptr) - 1] == '.' || ptr[strlen(ptr) - 1] == ',') // 뒤에 문자 제거
        ptr[strlen(ptr) - 1] = '\0';
    if (ptr[strlen(ptr) - 1] == '?' || ptr[strlen(ptr) - 1] == '!')
        ptr[strlen(ptr) - 1] = '\0';

    if (ptr[strlen(ptr) - 1] == 's' && strlen(ptr) > 3) // 3인칭 단수 또는 복수형에 s 제거
        ptr[strlen(ptr) - 1] = '\0';

    if (ptr[0] < 97) // 앞에 대문자를 소문자로 변경
        ptr[0] += 32;

    return ptr;
}

int Exc_check(char* ptr)
{
    for (int k = 0; k < 20; k++) // 예외단어 검사
    {
        if (strcmp(ptr, exc[k]) == 0)
            return 1;
    }
    return 0;
}

int Word_check(word_dic* words, char* ptr)
{
    for (int k = 0; k < DIC_SIZE; k++)
    {
        if (words[k].word != NULL)
            if (strcmp(ptr, words[k].word) == 0)
            {
                return k;
            }
    }
    return 0;
}

word_dic* Word_insert(word_dic* words, char* ptr, int line_no)
{
    int loc;
    int index = 0;

    if (loc = Word_check(words, ptr)) // 이미 사전에 등록되어 있는 경우
    {
        while (words[loc].line_num[index] != 0)
        {
            index++;
        }
        words[loc].line_num[index] = line_no;
        return words;
    }
    // 아직 사전에 등록 안된 경우

    strcpy(words[word_count].word, ptr);
    words[word_count].line_num[0] = line_no;
    word_count++;
    return words;
}

int Word_search(word_dic* words, char* check_word)
{
    for (int i = 0; i < word_count; i++)
    {
        const char* temp = words[i].word;
        if (strcmp(temp, check_word) == 0)
        {
            printf("%s를 검색합니다\n", check_word);
            printf("%s\n", temp);

            int k = 0;
            while (words[i].line_num[k] != 0)
            {
                printf("%dth line  ", words[i].line_num[k]);
                k++;
            }
            printf("\n\n");
            break;
        }
    };
}

int compare(const void* a, const void* b)
{
    const word_dic* m1 = (const word_dic*)a;
    const word_dic* m2 = (const word_dic*)b;
    return strcmp(m1->word, m2->word);
}

int main() {
    int line_no = 0; // 라인 번호
    char line[LINE_SIZE]; // 한 줄씩 읽어올 공간

    word_dic words[DIC_SIZE]; // 단어와 색인을 저장할 구조체 배열

    for (int i = 0; i < DIC_SIZE; i++) // 단어 공간에 동적 메모리 할당
    {
        words[i].word = malloc(sizeof(char) * 10);
        for (int k = 0; k < 15; k++)
        {
            words[i].line_num[k] = 0;
        }
    }
    
    FILE* fp = fopen("textfile.txt", "r");
    if (fp == NULL)
        printf("입력파일을 열 수 없습니다.\n");


    if (fp != NULL) {
        while (!feof(fp)) {
            fgets(line, sizeof(line), fp);
            line_no++;  // 총 6줄
            printf("-----%d번 줄-----\n", line_no);
            printf(line);
            char* ptr = strtok(line, " ");

            while (ptr != NULL) {

                Word_make(ptr);
                printf("\n%s", ptr);

                if (Exc_check(ptr))
                {
                    printf(" -> 제외됨");
                    ptr = strtok(NULL, " ");
                    continue;
                }

                Word_insert(words, ptr, line_no);

                ptr = strtok(NULL, " "); // 다음 단어 파싱하기
            }
            printf("\n\n");
        }


        fclose(fp);
    }

    printf("-----정렬 전 목록-----\n");
    for (int i = 0; i < word_count; i++)
    {

        printf("%s\n", words[i].word);
    }

    qsort(words, word_count, sizeof(words[0]), compare);
    printf("-----정렬 후 목록-----\n");
    for (int i = 0; i < word_count; i++)
    {
        
        printf("%s\n", words[i].word);
    }

    printf("\n저장된 단어 갯수: %d\n\n", word_count);

    Word_search(words, "end");
    Word_search(words, "zoo");

    return 0;
}