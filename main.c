#include <stdio.h>
#include <string.h>
#include <time.h>
#include <json-c/json.h>

#define MAX_QUIZZES 100

typedef struct {
    char question[100];
    char options[4][50];
    int answer;
} Quiz;

void decryptGPG(const char *encryptedFile, char **decryptedString) {
    char command[256];
    snprintf(command, sizeof(command), "gpg --batch --yes --decrypt %s", encryptedFile);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        printf("GPG 복호화 명령을 실행하는 데 실패했습니다\n");
        exit(1);
    }

    char buffer[1024];
    size_t totalSize = 0;
    *decryptedString = NULL;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t len = strlen(buffer);
        *decryptedString = (char *)realloc(*decryptedString, totalSize + len + 1);
        strcpy(*decryptedString + totalSize, buffer);
        totalSize += len;
    }

    pclose(pipe);

    if (*decryptedString == NULL) {
        printf("복호화가 실패했거나 빈 결과가 나왔습니다\n");
        exit(1);
    }
}

void shuffle(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));

    char *jsonString;
    decryptGPG("quizzes.json.gpg", &jsonString);

    struct json_object *quizzesJson;
    struct json_object *questionJson;
    struct json_object *optionsJson;
    struct json_object *answerJson;

    struct json_object* parsedJson = json_tokener_parse(jsonString);
    free(jsonString);

    json_object_object_get_ex(parsedJson, "quizzes", &quizzesJson);
    const size_t numQuizzes = json_object_array_length(quizzesJson);

    Quiz quizzes[MAX_QUIZZES];

    for (size_t i = 0; i < numQuizzes; i++) {
        struct json_object* quizJson = json_object_array_get_idx(quizzesJson, i);

        json_object_object_get_ex(quizJson, "question", &questionJson);
        json_object_object_get_ex(quizJson, "options", &optionsJson);
        json_object_object_get_ex(quizJson, "answer", &answerJson);

        strncpy(quizzes[i].question, json_object_get_string(questionJson), sizeof(quizzes[i].question) - 1);
        quizzes[i].question[sizeof(quizzes[i].question) - 1] = '\0';

        for (size_t j = 0; j < 4; j++) {
            struct json_object *optionJson = json_object_array_get_idx(optionsJson, j);
            strncpy(quizzes[i].options[j], json_object_get_string(optionJson), sizeof(quizzes[i].options[j]) - 1);
            quizzes[i].options[j][sizeof(quizzes[i].options[j]) - 1] = '\0';
        }

        quizzes[i].answer = json_object_get_int(answerJson);
    }

    int numQuestions = (rand() % numQuizzes) + 1;
    printf("총 %d개의 문제를 출제합니다.\n", numQuestions);

    if (numQuestions < 1 || numQuestions > numQuizzes) {
        printf("잘못된 문제 수입니다. 종료합니다.\n");
        json_object_put(parsedJson);
        return 1;
    }

    int indices[MAX_QUIZZES];
    for (size_t i = 0; i < numQuizzes; i++) {
        indices[i] = i;
    }

    shuffle(indices, numQuizzes);

    int userAnswer;
    int score = 0;

    for (int i = 0; i < numQuestions; i++) {
        int quizIndex = indices[i];
        printf("%s\n", quizzes[quizIndex].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, quizzes[quizIndex].options[j]);
        }

        printf("정답을 입력하세요: ");
        scanf("%d", &userAnswer);

        if (userAnswer - 1 == quizzes[quizIndex].answer) {
            printf("정답입니다!\n");
            score++;
        } else {
            printf("오답입니다! 정답은 %s 입니다.\n", quizzes[quizIndex].options[quizzes[quizIndex].answer]);
        }
        printf("\n");
    }

    printf("총 %d문제 중 %d문제를 맞추셨습니다.\n", numQuestions, score);

    json_object_put(parsedJson);

    return 0;
}