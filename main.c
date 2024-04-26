#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define POS_X1 35	//��ӡ�˵���λ���
#define POS_X3 50
#define POS_X4 60
#define MAX_LENGTH 300  //��������Ԫ�ر������ַ�������

typedef char E;
typedef int T;

//���������ڵ�
typedef struct TreeNode {
    E element;  //Ԫ��
    T value;    //Ȩֵ
    struct TreeNode * left;	//���ֽڵ�
    struct TreeNode * right;	//���ֽڵ�
    struct TreeNode * next; //����ָ��
} * Node;

//���ȶ���
typedef struct Queue {
    Node front, rear;  //�������ȶ���
} * LinkedQueue;

//��������
void SetPosition(int x, int y); //��λ���λ��
int Menu(void); //��ӡ�˵�
int InitQueue(LinkedQueue queue);   //��ʼ�����ȶ���
int OfferQueue(LinkedQueue queue, T value, E element);  //���ȶ������
Node PollQueue(LinkedQueue queue);  //���ȶ��г���
Node CreateNode(E element, T value);    //�������������ڵ�
int OfferNode(LinkedQueue queue, Node node);    //���������ڵ�������ȶ���
void CreatHfmTree(LinkedQueue queue, int n);    //�����������
int OfferQueueP(LinkedQueue queue, Node root);  //��ʱ���ȶ������
int IsEmpty(LinkedQueue queue); //�ж϶����Ƿ�Ϊ��
void LevelOrderToFile(Node root);  //���������������д���ļ�
Node ReadHfmTree(LinkedQueue queue);    //���ļ���ȡ��������
char * EnCodeSingle(Node root, E e);  //���뵥���ַ�e
void PrintEncode(Node root, E e);   //�������ַ�д���ļ�
void EnCoding(Node root);   //�����ļ�
void DeCoding(Node root);   //�����ļ�
void PrePrint(Node root, int num);  //ǰ�������ӡ�������������
void CodeFilePrint(Node root);  //��ȡ�����ļ������д���ļ�

void SetPosition(int x, int y) {
    HANDLE hout;
    COORD pos;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hout, pos);
}

int Menu(void) {
    int posy = 5;
    int option;
    int i, j;
    SetPosition(POS_X3, posy);
    printf("������������\n");
    for (i = 0; i < 2; i++) {
        SetPosition(POS_X1, ++posy);
        for (j = 0; j < 55; j++) {
            printf("-");
        }
    }
    SetPosition(POS_X1, ++posy);
    printf("1.������������");
    SetPosition(POS_X4, posy);
    printf("2.���뵥��Ԫ��");
    SetPosition(POS_X1, posy += 2);
    printf("3.�����ļ�");
    SetPosition(POS_X4, posy);
    printf("4.�����ļ�");
    SetPosition(POS_X1, posy += 2);
    printf("5.��ӡ��������");
    SetPosition(POS_X4, posy);
    printf("6.��ȡ��������");
    SetPosition(POS_X1, posy += 2);
    printf("7.��ӡ�����ļ�");
    SetPosition(POS_X4, posy);
    printf("0.�˳�");
    for (i = 0; i < 2; i++) {
        SetPosition(POS_X1, ++posy);
        for (j = 0; j < 55; j++) {
            printf("-");
        }
    }
    SetPosition(POS_X1, ++posy);
    printf("��ѡ����Ĳ���[0~7]:[  ]\b\b");
    scanf("%d", &option);
    return option;
}

//��ʼ�����ȶ���
int InitQueue(LinkedQueue queue) {
    Node node = (Node)malloc(sizeof(struct TreeNode));
    if(node == NULL) return 0;
    queue->front = queue->rear = node;
    node->left = node->right = NULL;
    node->next = NULL;   //�ÿ�
    return 1;
}

//�����ȶ���
int OfferQueue(LinkedQueue queue, T value, E element){
    Node node = (Node)malloc(sizeof(struct TreeNode));
    if(node == NULL) return 0;
    node->element = element;
    node->value = value;
    node->next = NULL;
    node->left = node->right = NULL;
    Node pre = queue->front;
    while (pre->next && pre->next->value <= value)   //���뵽���ʵ�λ��
        pre = pre->next;
    if(pre == queue->rear) {
        queue->rear->next = node;
        queue->rear = node;
    } else {
        node->next = pre->next;
        pre->next = node;
    }
    return 1;
}

//����
Node PollQueue(LinkedQueue queue){
    Node node = queue->front->next;
    queue->front->next = queue->front->next->next;  //ֱ��ȡ��
    if(queue->rear == node) queue->rear = queue->front;   //�ж��ǲ������һ��
    return node;
}

//�������������ڵ�
Node CreateNode(E element, T value){
    Node node = (Node)malloc(sizeof(struct TreeNode));
    node->element = element;
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

//�����������ڵ����
int OfferNode(LinkedQueue queue, Node node){
    node->next = NULL;
    Node pre = queue->front;
    while (pre->next && pre->next->value <= node->value)   //
        pre = pre->next;
    if(pre == queue->rear) {
        queue->rear->next = node;
        queue->rear = node;
    } else {
        node->next = pre->next;
        pre->next = node;
    }
    return 1;
}

//������������
void CreatHfmTree(LinkedQueue queue, int n) {
    T value;
    E element;
    for (int i = 0; i < n; ++i) {
        printf("������ڵ�Ȩֵ�����ƣ�");
        scanf("%d %c", &value, &element);
        OfferQueue(queue, value, element);
    }
    while (queue->front != NULL && queue->front->next != queue->rear) {   //
        Node left = PollQueue(queue);
        Node right = PollQueue(queue);
        Node node = CreateNode(' ', left->value + right->value);   //
        node->left = left;
        node->right = right;
        OfferNode(queue, node);
    }
    printf("�������������ɹ���\n");
    getchar();
}

//�������
int OfferQueueP(LinkedQueue queue, Node root){
    queue->rear->next = root;
    queue->rear = root;
    return 1;
}

//�ж϶����Ƿ�Ϊ��
int IsEmpty(LinkedQueue queue){
    return queue->front == queue->rear;
}

//�������д���ļ�HfmTree
void LevelOrderToFile(Node root){
    FILE* f = fopen("D:\\Clion\\Projects\\HfmTree\\HfmTree.txt", "w");
    struct Queue queue_p;   //����Ķ���
    InitQueue(&queue_p);
    OfferQueueP(&queue_p, root);  //�����
    while (!IsEmpty(&queue_p)) {   //�п�
        Node node = PollQueue(&queue_p);   //����
        fprintf(f, "%c, %d\n", node->element, node->value);
        if(node->left)    //�ȿ����
            OfferQueueP(&queue_p, node->left);  //���
        if(node->right){	//�ٿ��ұ�
            OfferQueueP(&queue_p, node->right);
        }
    }
    fclose(f);
}

//���ļ�HfmTree�ж�ȡ��������
Node ReadHfmTree(LinkedQueue queue) {
    // ���ļ���ȷ���ļ�����ȷ
    FILE* file = fopen("D:\\Clion\\Projects\\HfmTree\\HfmTree.txt", "r");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        return NULL;
    }

    char line[50];  // ����ÿ�в�����50���ַ�
    while (fgets(line, sizeof(line), file)) {
        // ȥ����ĩβ�Ļ��з�
        line[strcspn(line, "\n")] = '\0';

        // ������Ƿ�Ϊ��
        if (line[0] == ' ') {
            continue;  // ��������
        }

        char element;
        int value;
        sscanf(line, "%c, %d", &element, &value);  // ��ÿ������ȡ�ַ���Ȩ��ֵ
        OfferQueue(queue, value, element);  // �������OfferQueue����
    }

    fclose(file);  // �ر��ļ�

    //������������
    while (queue->front != NULL && queue->front->next != queue->rear) {
        Node left = PollQueue(queue);
        Node right = PollQueue(queue);
        Node node = CreateNode(' ', left->value + right->value);   //�������ڵ�
        node->left = left;
        node->right = right;
        OfferNode(queue, node);
    }

    Node root = PollQueue(queue);
    printf("����������ȡ�ɹ���\n");
    return root;
}

//���뵥��Ԫ��e
char * EnCodeSingle(Node root, E e){
    if(root == NULL) return NULL;   //����ڵ�û0�򷵻�NULL
    if(root->element == e) return "";   //����ҵ�Ҫ��Ԫ���򷵻�""
    char *str = EnCodeSingle(root->left, e);
    char *s = malloc(sizeof (char));	//�����ڴ�
    if(str != NULL) {
        s[0] = '0';
        str = strcat(s, str);   //���ƴ��
    } else {    //�������������
        str = EnCodeSingle(root->right, e);	//���ұ���
        if(str != NULL) {
            s[0] = '1';
            str = strcat(s, str);   //���ƴ��
        }
    }
    return str;   //���ر���
}

//������Ԫ��д���ļ�CodeFile
void PrintEncode(Node root, E e){
    printf("%c �ı���Ϊ��%s", e, EnCodeSingle(root, e));	//��ӡ������̨
    FILE *CodeFile = fopen("D:\\Clion\\Projects\\HfmTree\\CodeFile.txt", "a+");
    fprintf(CodeFile, "%s", EnCodeSingle(root, e));
    fclose(CodeFile);
    putchar('\n');
}

//���ļ�ToBeTran�б���Ԫ��
void EnCoding(Node root) {
    FILE *ToBeTran = fopen("D:\\Clion\\Projects\\HfmTree\\ToBeTran.txt", "r+");
    FILE *CodeFile = fopen("D:\\Clion\\Projects\\HfmTree\\CodeFile.txt", "w+");
    if (feof(ToBeTran)){
        printf("ToBeTran�ļ�Ϊ��!");
        return ;
    }

    char c;
    while (!feof(ToBeTran)) {
        c = fgetc(ToBeTran);
        if (feof(ToBeTran))
            break;
        PrintEncode(root, c);
    }
    fclose(ToBeTran);

    //��ȡ�������ļ�����
    char buffer[256];
    fgets(buffer, sizeof(buffer), CodeFile);
    printf("������ļ�����Ϊ: %s\n", buffer);
    fclose(CodeFile);
}

//�����ļ�CodeFile�ַ�
void DeCoding(Node root) {
    Node node = root;
    char line[100];
    FILE *CodeFile = fopen("D:\\Clion\\Projects\\HfmTree\\CodeFile.txt", "r");
    fgets(line, 100, CodeFile);
    int len = strlen(line);
    printf("�ļ�CodeFileΪ��%s\n", line);
    for (int i = 0; i < len; ++i) {
        if(line[i] != '0' && line[i] !='1') {
            printf("�ļ�����");
            return;
        }
    }
    printf("���ļ�CodeFile�б�����ַ�Ϊ��");
    for (int i = 0; i < len; ++i) {
        if(line[i] == '0'){
            node = node->left;
            if(!node->left) {
                printf("%c", node->element);
                node = root;
            }
        }else if (line[i] == '1') {
            node = node->right;
            if(!node->right) {
                printf("%c", node->element);
                node = root;
            }
        }
    }
    printf("\n");
}

//ǰ�������ӡ���������İ����
void PrePrint(Node root, int num){
    if(root == NULL) return;
    FILE *TreePrint = fopen("D:\\Clion\\Projects\\HfmTree\\TreePrint.txt", "a");
    for (int i = 0; i < num; ++i) {
        printf("-");
        fprintf(TreePrint, "-");
    }
    printf("%3d %c", root->value, root->element);
    fprintf(TreePrint,"%3d %c", root->value, root->element);
    printf("\n");
    fprintf(TreePrint,"\n");
    PrePrint(root->left, num-8);
    PrePrint(root->right, num-8);
    fclose(TreePrint);
}

void CodeFilePrint(Node root) {
    char lines[MAX_LENGTH];	//����洢�������ַ���
    FILE *CodeFile = fopen("D:\\Clion\\Projects\\HfmTree\\CodeFile.txt", "r");
    FILE *CodePrint = fopen("D:\\Clion\\Projects\\HfmTree\\CodePrint.txt", "w+");

    fgets(lines, MAX_LENGTH, CodeFile);

    printf("�ļ�CodeFile����Ϊ��\n");
    for (int i = 0; i < strlen(lines); ++i) {
        //ÿ����ʾ50���ַ�
        if(i % 50 == 0) {
            printf("\n");
        }
        printf("%c", lines[i]);
    }
    printf("\n");

    Node node = root;
    int len = strlen(lines);
    printf("CodeFile�ļ�������������£�\n");
    for (int i = 0; i < len; ++i) {
        if(lines[i] == '0'){
            node = node->left;
            if(!node->left) {
                printf("%c", node->element);
                fprintf(CodePrint, "%c", node->element);
                node = root;
            }
        }else if (lines[i] == '1') {
            node = node->right;
            if(!node->right) {
                printf("%c", node->element);
                fprintf(CodePrint, "%c", node->element);
                node = root;
            }
        }
    }

    printf("\nCodeFile�ļ�����������ѳɹ�д��CodePrint!\n");
    fclose(CodeFile);
    fclose(CodePrint);
}

int main() {
    int n, ch;
    char element;
    int first = 1;
    struct Queue queue; //�������ȶ���
    InitQueue(&queue);
    Node root;

    system("mode con cols=130 lines=60");
    system("color 0E");

    while (1) {
        system("cls");
        ch = Menu();
        printf("��ѡ����Ĳ���[0~7]:[  ]\b\b");
        switch (ch) {
            case 1:
                system("cls");
                printf("������ڵ������ ");
                scanf("%d", &n);
                CreatHfmTree(&queue, n);
                first = 0;
                root = PollQueue(&queue);
                LevelOrderToFile(root);
                system("pause");
                break;
            case 2:
                system("cls");
                if (first) {
                    printf("����������δ������\n");
                    printf("�Ƿ���ļ��ж�ȡ����y/n��\n");
                    getchar();
                    char a;
                    scanf("%c", &a);
                    if(a == 'y') {
                        root = ReadHfmTree(&queue);
                        first = 0;
                    }
                    system("pause");
                    break;
                }
                printf("������Ҫ�����Ԫ�أ�");
                getchar();
                scanf("%c", &element);
                PrintEncode(root, element);
                system("pause");
                break;
            case 3:
                system("cls");
                if (first) {
                    printf("����������δ������\n");
                    printf("�Ƿ���ļ��ж�ȡ����y/n��\n");
                    getchar();
                    char a;
                    scanf("%c", &a);
                    if(a == 'y') {
                        root = ReadHfmTree(&queue);
                        first = 0;
                    }
                    system("pause");
                    break;
                }
                EnCoding(root);
                system("pause");
                break;
            case 4:
                system("cls");
                if (first) {
                    printf("����������δ������\n");
                    printf("�Ƿ���ļ��ж�ȡ����y/n��\n");
                    getchar();
                    char a;
                    scanf("%c", &a);
                    if(a == 'y') {
                        root = ReadHfmTree(&queue);
                        first = 0;
                    }
                    system("pause");
                    break;
                }
                DeCoding(root);
                system("pause");
                break;
            case 5:
                system("cls");
                if (first) {
                    printf("����������δ������\n");
                    printf("�Ƿ���ļ��ж�ȡ����y/n��\n");
                    getchar();
                    char a;
                    scanf("%c", &a);
                    if(a == 'y') {
                        root = ReadHfmTree(&queue);
                        first = 0;
                    }
                    system("pause");
                    break;
                }
                FILE *temp = fopen("D:\\Clion\\Projects\\HfmTree\\TreePrint.txt", "w");
                if (temp) fclose(temp);
                printf("����������������£�\n");
                PrePrint(root, 50);
                system("pause");
                break;
            case 6:
                system("cls");
                root = ReadHfmTree(&queue);
                printf("��ȡ�����������ڵ�ֵΪ: %d\n",root->value);
                first = 0;
                system("pause");
                break;
            case 7:
                system("cls");
                if (first) {
                    printf("����������δ������\n");
                    printf("�Ƿ���ļ��ж�ȡ����y/n��\n");
                    getchar();
                    char a;
                    scanf("%c", &a);
                    if(a == 'y') {
                        root = ReadHfmTree(&queue);
                        first = 0;
                    }
                    system("pause");
                    break;
                }
                CodeFilePrint(root);
                system("pause");
                break;
            case 0:
                system("cls");
                printf("ϵͳ�ѳɹ��˳���\n");
                exit(0);
            default:
                system("cls");
                printf("�����������������룡\n");
                system("pause");
        }
    }
    return 0;
}