#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
using namespace std;

struct aeroflot {
    int number;
    char departure[30];
    char destination[30];
    int flight_num;
    char plane_type[30];
};

struct tree{
	aeroflot data;
	tree *left;
	tree *right;
};

struct aeroflot vvod();//ввод данных
struct tree* org_tree();//организации дерева
void prosmotr(tree* top);//просмотра (обхода) дерева
void otobr(tree* top, int otstup);//отображения структуры дерева
struct tree* add_tree(tree* top, const aeroflot& newtree);//добавления узла в дерево
struct tree* delete_node(tree* top, int val);//исключения узла из дерева
struct tree* find_min(struct tree* top);//поиск узла с минимальным значением в дереве
void save_tree(tree* top, FILE* file);//сохранения в файл
struct tree* load_tree(FILE* file);//загрузки данных из файла и создания по этим данным нового дерева
void remove_tree(tree* top);//освобождения динамической памяти, занимаемой деревом (обязательно вызывать при выходе из программы)
void find_flights(tree* top, char* destination);

int main() {
	FILE* file;
	struct tree* top=NULL;
	struct tree* tmp=NULL;
	while (1) {
		int answer, route;
			printf("1. Add node\n"
			"2. Print tree\n"
			"3. Org tree\n"
			"4. Otobr tree\n"
			"5. Delete node\n"
			"6. Save into file\n"
			"7. Load from file\n"
			"8. Print leftmost node\n"
			"9. Find flights\n"
			"0. Exit\n");
			printf("\nEnter -> "),scanf("%d",&answer);
			switch (answer) {
				case 0:
					remove_tree(top);
					top=NULL;
					remove_tree(tmp);
					tmp=NULL;
					return 0;
				case 1:
					top=add_tree(top, vvod());
					break;
				case 2:
					if (top) {
						prosmotr(top);
					} else printf("The tree is clear\n");
					break;
				case 3:
					remove_tree(top);
					top=NULL;
					top=org_tree();
					break;
				case 4:
					if (top) {
						otobr(top, 3);
					} else printf("The tree is clear\n");
					break;
				case 5:
					if (top) {
						printf("\nEnter route for removing -> "),scanf("%d",&route);
						top=delete_node(top, route);
					} else printf("The tree is clear\n");
					break;
				case 6:
					if (top) {
						file=fopen("tree.bin", "wb");
	    				if (file) {
	        				save_tree(top, file);
	        				fclose(file);
	    				}
	    				printf("Saving successful\n");
					} else printf("The tree is clear\n");
					break;
				case 7:
					file = fopen("tree.bin", "rb");
	    			if (file) {
	        			top=load_tree(file);
	        			fclose(file);
	    			}
	    			printf("Loading successful\n");
	    			break;
	    		case 8:
				    if (top) {
				        find_min(top);
				    }
				    else {
				        printf("The tree is clear\n");
				    }
				    break;
				case 9:
					if (top){
				        char dest[30];
				        printf("Enter destination: ");
				        scanf("%s", dest);
				        find_flights(top, dest);
			    	}
					else{
						printf("The tree is clear\n");
					}
					break;
				default:
					printf("The command isn't recognized\n");
			}
	}
}

struct aeroflot vvod() {
	struct aeroflot node;
	printf("\n Write index number >> ");
	scanf("%d", &node.number);
	printf("\n Write departure point >> ");
	scanf("%s", &node.departure);
	printf("\n Write destination point >> ");
	scanf("%s", &node.destination);
	printf("\n Write flight number >> ");
	scanf("%d", &node.flight_num);
	printf("\n Write type of flight >> ");
	scanf("%s", &node.plane_type);		
	puts("\n Data recorded.");
	return node;
}

void prosmotr(tree* top) { //обход прямой
	if (top) {
		printf("Number = %d\n",top->data.number);
		printf("Departure = %s\n",top->data.departure);
		printf("Destination = %s\n",top->data.destination);
		printf("Flight Number = %d\n",top->data.flight_num);
		printf("Plane Type = %s\n",top->data.plane_type);
		prosmotr(top->left);
		prosmotr(top->right);
	}
}

struct tree* add_tree(tree* top, const aeroflot& newtree) {
	if (!top) {
		top = new tree;
		top->data = newtree;
		top->left = NULL;
		top->right = NULL;
	} else {
		if (top->data.number > newtree.number) {
			top->left = add_tree(top->left, newtree);
		} else {
			top->right = add_tree(top->right, newtree);
		}
	}
	return top;
}

tree* org_tree() {
    tree* top=NULL;
    int wants_more = 1;
    while (wants_more) {
        top = add_tree(top, vvod());
        printf("Wants more? (exit - 0)\n");
        char answer = getch();
        if (answer == '0') {
            wants_more = 0;
        }
    }
    return top;
}

void otobr(tree* top, int otstup) {
	if (top) {
		otstup+=3;
		otobr(top->right,otstup);
		for(int i=0; i<otstup; i++) printf(" ");
		printf("%d\n",top->data.number);
		otobr(top->left,otstup);
	}
}

void remove_tree(tree* top) {
	if (top) {
		remove_tree(top->left);
		remove_tree(top->right);
		free(top);
	}
}

struct tree* delete_node(tree* top, int val) {
    if (top==NULL) {
        return top;
    }
    //Если значение меньше, идем в левое поддерево
    if (val<(top->data.number)) {
        top->left=delete_node(top->left, val);
    }
    //если значение больше, идем в правое поддерево
    else if (val>(top->data.number)) {
        top->right=delete_node(top->right, val);
    }
    //если нашли узел, который нужно удалить
    else {
        //если у узла нет потомков
        if (top->left==NULL && top->right==NULL) {
            free(top);
            top=NULL;
        }
        //если у узла только один потомок
        else if (top->left==NULL) {
            struct tree* temp=top;
            top=top->right;
            free(temp);
        }
        else if (top->right==NULL) {
            struct tree* temp=top;
            top=top->left;
            free(temp);
        }
        //если у узла есть два потомка
        else {
            struct tree* temp=find_min(top->right);
            top->data=temp->data;
            top->right=delete_node(top->right, temp->data.number);
        }
    }
    return top;
}

struct tree* find_min(struct tree* top) {
    struct tree* curr=top;
    while (curr && (curr->left!=NULL)) {
        curr=curr->left;
    }
    printf("Leftmost node:\n");
        printf("Number = %d\n", top->data.number);
        printf("Departure = %s\n", top->data.departure);
        printf("Destination = %s\n", top->data.destination);
        printf("Flight Number = %d\n", top->data.flight_num);
        printf("Plane Type = %s\n", top->data.plane_type);
    return curr;
}

void save_tree(tree* top, FILE* file) {
    if (top) {
        fwrite(&(top->data), sizeof(aeroflot), 1, file);
        save_tree(top->left, file);
        save_tree(top->right, file);
    }
}

struct tree* load_tree(FILE* file) {
    struct tree* top=NULL;
    struct aeroflot node;
    while (fread(&node, sizeof(aeroflot), 1, file)) {
        top=add_tree(top, node);
    }
    return top;
}

void find_flights(tree* top, char* destination) {
    int found = 0; // флаг, который становится в 1, если хотя бы один рейс был найден
    while (top) {
        if (strcmp(top->data.destination, destination) == 0) {
            printf("Flight number %d to %s\n", top->data.flight_num, top->data.destination);
            found = 1;
        }
        if (top->left) {
            find_flights(top->left, destination);
        }
        top = top->right;
    }
    if (!found) {
        printf("Flights not found\n");
    }
}
