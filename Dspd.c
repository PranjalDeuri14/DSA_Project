#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
	char *s;	//song_name
	char *l;	//Artist
	char *fa;	//album or film
	char *g;	//genre
	float d;	//duration 
    struct Node* left;
    struct Node* right;
    int height;  //height of the node
    int flag;    //flag for random call
};
//creating a node and assigning its value
struct Node* createNode(char* song_name, char* Artist, char* album, char* genre, float duration){
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->s = strdup(song_name);
    node->l = strdup(Artist);
    node->fa = strdup(album);
    node->g = strdup(genre);
    node->d = duration;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Help to get the height of the node
int getHeight(struct Node *n){
    if(n==NULL)
        return 0;
    return n->height;
}

// Help to get the greatest of a and b
int max (int a, int b){
    return (a>b)?a:b;
}

// Help to get the balance factor of nodes
int getBalanceFactor(struct Node * n){
    if(n==NULL){
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

//RightRotation
struct Node* rightRotate(struct Node* y){
    struct Node* x = y->left;
    struct Node* T2 = x->right;
 
    x->right = y;
    y->left = T2;
 
    x->height = max(getHeight(x->right), getHeight(x->left)) + 1;
    y->height = max(getHeight(y->right), getHeight(y->left)) + 1;
 
    return x;
}

//LeftRotation
struct Node* leftRotate(struct Node* x){
    struct Node* y = x->right;
    struct Node* T2 = y->left;
 
    y->left = x;
    x->right = T2;
 
    x->height = max(getHeight(x->right), getHeight(x->left)) + 1;
    y->height = max(getHeight(y->right), getHeight(y->left)) + 1;
 
    return y;
}

//Inserting data into nodes
struct Node *insert(struct Node* node, char* song_name, char* Artist, char* album, char* genre, float duration){
    FILE *fp;
    fp = fopen("file.txt", "a"); //open the file in append mode

    if (node == NULL){
        node = createNode(song_name, Artist, album, genre, duration);
        fprintf(fp, "%s %s %s %s %f\n", song_name, Artist, album, genre, duration); //write to file
    }
    else if (strcmp(song_name, node->s) < 0){
        node->left  = insert(node->left, song_name, Artist, album, genre, duration);
    }
    else if (strcmp(song_name, node->s) > 0){
        node->right = insert(node->right, song_name, Artist, album, genre, duration);
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int bf = getBalanceFactor(node);

    // Right Right Case
    if(bf>1 && strcmp(song_name, node->left->s) < 0){
        return rightRotate(node);
    }
    // Left Left Case
    if(bf<-1 && strcmp(song_name, node->right->s) > 0){
        return leftRotate(node);
    }
    // Left Right Case
    if(bf>1 && strcmp(song_name, node->left->s) > 0){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left Case
    if(bf<-1 && strcmp(song_name, node->right->s) < 0){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    fclose(fp); //close the file
    return node;
}

//Inserting data from file.txt
struct Node *insertFromFile(struct Node* node, char* fileName){
    FILE *fp;
    fp = fopen(fileName, "r"); // open the file in read mode

    char song_name[100], Artist[100], album[100], genre[100];
    float duration;

    while(fscanf(fp, "%s %s %s %s %f", song_name, Artist, album, genre, &duration) != EOF){
        node = insert(node, song_name, Artist, album, genre, duration); // insert the read values into the tree
    }

    fclose(fp); //close the file
    return node;
}

//Ascending order traversal
void inOrder(struct Node* root, int* index) {
    if (root != NULL) {
        inOrder(root->left, index);
        printf("%d. Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f\n", *index, root->s, root->l, root->fa, root->g, root->d);
        (*index)++;
        inOrder(root->right, index);
    }
}

//Descending order traversal
void DeOrder(struct Node* root, int* index){
    if(root!=NULL){
        DeOrder(root->right, index);
        printf("%d. Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", *index, root->s, root->l, root->fa, root->g, root->d);
        (*index)++;
        DeOrder(root->left, index);
    }
}

// minimum value of the current node
struct Node* minValueNode(struct Node *node) {
    struct Node *current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

//deleting Nodes with respect to their Song names
void deleteNodeWithSongName(struct Node **root, char *s) {
    if (*root == NULL) {
        return;
    }
    
    deleteNodeWithSongName(&((*root)->left), s);
    deleteNodeWithSongName(&((*root)->right), s);
    
    if (strcmp(s, (*root)->s) == 0) {
        struct Node *temp = *root;
        if ((*root)->left == NULL) {
            *root = (*root)->right;
        } 
        else if ((*root)->right == NULL) {
            *root = (*root)->left;
        } 
        else {
            struct Node *minRight = minValueNode((*root)->right);
            (*root)->s = minRight->s;
            (*root)->l = minRight->l;
            (*root)->fa = minRight->fa;
            (*root)->g = minRight->g;
            (*root)->d = minRight->d;
            temp = minRight;
            deleteNodeWithSongName(&((*root)->right), minRight->l);
        }
        free(temp->s);
        free(temp->l);
        free(temp->fa);
        free(temp->g);
        free(temp);
        printf("!!!Deletion Complete!!!");
    }
}

//deleting Nodes with respect to their Artist
void deleteNodesWithArtist(struct Node **root, char *l) {
    if (*root == NULL) {
        return;
    }
    
    deleteNodesWithArtist(&((*root)->left), l);
    deleteNodesWithArtist(&((*root)->right), l);
    
    if (strcmp(l, (*root)->l) == 0) {
        struct Node *temp = *root;
        if ((*root)->left == NULL) {
            *root = (*root)->right;
        } 
        else if ((*root)->right == NULL) {
            *root = (*root)->left;
        } 
        else {
            struct Node *minRight = minValueNode((*root)->right);
            (*root)->s = minRight->s;
            (*root)->l = minRight->l;
            (*root)->fa = minRight->fa;
            (*root)->g = minRight->g;
            (*root)->d = minRight->d;
            temp = minRight;
            deleteNodeWithSongName(&((*root)->right), minRight->l);
        }
        free(temp->s);
        free(temp->l);
        free(temp->fa);
        free(temp->g);
        free(temp);
        printf("!!!Deletion Complete!!!");
    }
}

//Printing a playlist with respect to their Artist
void newPlayArt(struct Node** root, char* Artist){
    if((*root)!=NULL){
        newPlayArt(&((*root)->left), Artist);
        if(strcmp(Artist, (*root)->l) == 0){
            printf(" Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", (*root)->s, (*root)->l, (*root)->fa, (*root)->g, (*root)->d);
        }
        newPlayArt(&((*root)->right), Artist);
    }
}

//Printing a playlist with respect to their Genre
void newPlayGen(struct Node** root, char* genre){
    if((*root)!=NULL){
        newPlayGen(&((*root)->left), genre);
        if(strcmp(genre, (*root)->g) == 0){
            printf(" Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", (*root)->s, (*root)->l, (*root)->fa, (*root)->g, (*root)->d);
        }
        newPlayGen(&((*root)->right), genre);
    }
}

//Printing a playlist with respect to their Album
void newPlayAlb(struct Node** root, char* album){
    if((*root)!=NULL){
        newPlayAlb(&((*root)->left), album);
        if(strcmp(album, (*root)->fa) == 0){
            printf(" Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", (*root)->s, (*root)->l, (*root)->fa, (*root)->g, (*root)->d);
        }
        newPlayAlb(&((*root)->right), album);
    }
}

//Printing a playlist with respect to their Duration
void newPlayDu(struct Node** root, float duration){
    if((*root)!=NULL){
        newPlayDu(&((*root)->left), duration);
        if(duration== (*root)->d) {
            printf(" Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", (*root)->s, (*root)->l, (*root)->fa, (*root)->g, (*root)->d);
        }
        newPlayDu(&((*root)->right), duration);
    }
}

//Printing a playlist with respect to their Artist and Album
void newPlayMix(struct Node** root, char* Artist, char* album){
    if((*root)!=NULL){
        newPlayMix(&((*root)->left), Artist, album);
        if(strcmp(Artist, (*root)->l) == 0 && strcmp(album, (*root)->fa) == 0){
            printf(" Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", (*root)->s, (*root)->l, (*root)->fa, (*root)->g, (*root)->d);
        }
        newPlayMix(&((*root)->right), Artist, album);
    }
}

//Printing a playlist with respect to their Artist and Genre
void newPlaySinGen(struct Node** node, char* artistName, char* songGenre){
    if((*node)!=NULL){
        newPlaySinGen(&((*node)->left), artistName, songGenre);
        if(strcmp(artistName, (*node)->l) == 0 && strcmp(songGenre, (*node)->g) == 0){
            printf(" Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", (*node)->s, (*node)->l, (*node)->fa, (*node)->g, (*node)->d);
        }
        newPlaySinGen(&((*node)->right), artistName, songGenre);
    }
}

// Function to count the number of nodes in a binary search tree
int countNodes(struct Node* root)
{
    if (root == NULL) {
        return 0;
    }
    
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Function to find the kth smallest node in a binary search tree
struct Node* kthSmallest(struct Node* root, int k)
{
    if (root == NULL) {
        return NULL;
    }
    
    int count = countNodes(root->left);
    
    if (k == count + 1) {
        return root;
    } else if (k <= count) {
        return kthSmallest(root->left, k);
    } else {
        return kthSmallest(root->right, k - count - 1);
    }
}

void display(struct Node* root, int n, int c)
{
    // Count the number of songs in the tree
    int count = countNodes(root);
    
    if (count < n) {
        printf("Invalid song number.\n");
        return;
    }
    
    // Find the nth song in the tree
    struct Node* curr = kthSmallest(root, n);
    
    // Print the details of the current song
    printf("Current Song:- Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", curr->s, curr->l, curr->fa, curr->g, curr->d);
    
    // Print the details of the previous song (if c is 0 and there is a previous song)
    if (c == 0 && n > 1) {
        struct Node* prev = kthSmallest(root, n - 1);
        printf("Previous Song:- Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", prev->s, prev->l, prev->fa, prev->g, prev->d);
    }
    
    // Print the details of the next song (if c is 1 and there is a next song)
    if (c == 1 && n < count) {
        struct Node* next = kthSmallest(root, n + 1);
        printf("Next Song:- Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n", next->s, next->l, next->fa, next->g, next->d);
    }
}

// Function to display the details of the current song
void displayCurrentSong(struct Node* root, int serialNumber) {
    struct Node* currentSong = kthSmallest(root, serialNumber);
    printf("Current Song Details:\n");
    printf("Serial Number: %d\n", serialNumber);
    printf("Name: %s\n", currentSong->s);
    printf("Artist: %s\n", currentSong->l);
    printf("Duration: %.2f\n", currentSong->d);
}

// Function to display the details of the kth previous song
void displayPreviousSong(struct Node* root, int serialNumber, int k) {
    struct Node* previousSong = kthSmallest(root, serialNumber - k);
    printf("Previous Song Details:\n");
    printf("Serial Number: %d\n", serialNumber - k);
    printf("Name: %s\n", previousSong->s);
    printf("Artist: %s\n", previousSong->l);
    printf("Duration: %.2f\n", previousSong->d);
}

// Function to display the details of the kth next song
void displayNextSong(struct Node* root, int serialNumber, int k) {
    struct Node* nextSong = kthSmallest(root, serialNumber + k);
    printf("Next Song Details:\n");
    printf("Serial Number: %d\n", serialNumber + k);
    printf("Name: %s\n", nextSong->s);
    printf("Artist: %s\n", nextSong->l);
    printf("Duration: %.2f\n", nextSong->d);
}
// Function to display the details of the current song and 
//return back the data of the current song so to find the songs matching its attributes.
struct Node* CurrentSong(struct Node* root, int serialNumber) {
    struct Node* currentSong = kthSmallest(root, serialNumber);
    printf("Current Song Details:\n");
    printf("Serial Number: %d\n", serialNumber);
    printf("Name: %s\n", currentSong->s);
    printf("Artist: %s\n", currentSong->l);
    printf("Duration: %.2f\n", currentSong->d);
    return currentSong;
}

// Helper function to find all songs in the playlist that match the specified attributes
//Part of shuffle
void findMatchingSongs(struct Node* node, char* artist, char* album, char* genre, struct Node** matchingSongs, int* matchingCount) {
    if (node != NULL) {
        if (strcmp(node->l, artist) == 0 && strcmp(node->fa, album) == 0 && strcmp(node->g, genre) == 0 && node->flag == 0) {
            // Add the matching song to the array
            matchingSongs[*matchingCount] = node;
            *matchingCount += 1;
        }
        // Traverse left subtree
        findMatchingSongs(node->left, artist, album, genre, matchingSongs, matchingCount);
        // Traverse right subtree
        findMatchingSongs(node->right, artist, album, genre, matchingSongs, matchingCount);
    }
}


void shuffle(struct Node* root, char* artist, char* album, char* genre) {
    // Count number of songs in the playlist
    int songCount = countNodes(root);
    // Initialize an array to store the songs that match the specified attributes
    struct Node* matchingSongs[songCount];
    int matchingCount = 0;
    // Traverse the playlist and find all songs that match the specified attributes
    findMatchingSongs(root, artist, album, genre, matchingSongs, &matchingCount);
    // If no matching songs were found, return
    if (matchingCount == 0) {
        printf("No songs found matching the specified attributes.\n");
        return;
    }
    // Select a random song from the matching songs
    int randomIndex = rand() % matchingCount;
    struct Node* selectedSong = matchingSongs[randomIndex];
    // Mark the selected song using a flag
    selectedSong->flag = 1;
    // Print information about the selected song
    printf("Selected song: %s | Artist: %s | Album/Film: %s | Genre: %s | Duration: %.2f\n", 
           selectedSong->s, selectedSong->l, selectedSong->fa, selectedSong->g, selectedSong->d);
}


//Helps to find the songs between singer1 & singer2
void Range(struct Node* root, char *singer1, char *singer2) {
    if (root == NULL) {
        return;
    }

    // Search left subtree if singer1 is less than current node's singer name
    if (strcmp(singer1, root->l) < 0) {
        Range(root->left, singer1, singer2);
    }

    // Print current node's song if singer name is in between singer1 and singer2
    if (strcmp(singer1, root->l) < 0 && strcmp(singer2, root->l) > 0) {
        printf("Song name: %s | Artist: %s | Album: %s | Genre: %s | Duration: %.2f \n\n", root->s, root->l, root->fa, root->g, root->d);
    }

    // Search right subtree if singer2 is greater than current node's singer name
    if (strcmp(singer2, root->l) > 0) {
        Range(root->right, singer1, singer2);
    }
}


int main(){
    
    int inp,i=1;
    struct Node * root = NULL;
    root = insertFromFile(root, "file.txt");

    while(i>0)
    {
        printf("\n");
        printf("Press 1 To Insert Data.\n"); //Q1
        //Q2 I have not done the Q2 because in Q2 i have to remove duplicate.
        //As we know in AVL tree we can't have duplicate. So i didn't make Q2.
        printf("Press 2 To Print Data Ascending.\n");      //Q3
        printf("Press 3 To Print Data Descending.\n");     //Q3
        printf("Press 4 To Delete Data by Song Name.\n");  //Q4
        printf("Press 5 To Delete Data by Artist Name.\n");//Q4
        printf("Press 6 To Make a Playlist.\n");           //Q5
        printf("Press 7 To See The Prev and Next of a Song.\n");//Q6
        printf("Press 8 To See Kth Up or Down.\n");//Q7
        printf("Press 9 To See The Songs Attributes Matching With Current Song.\n");//Q8
        printf("Press 10 To Filter The Playlist.\n");//Q9
        printf("Printf 11 To Get Random Songs.\n");//Q10
        printf("Printf 12 To Get Range-Search.\n");//Q11
        printf("Press 0 To EXIT.\n");
        printf("\n");
        printf("Enter Your Choice: ");
        scanf("%d",&inp);
    	
    	if(inp==1){
    	    int num;
    	    printf("\n");
            printf("HOW MANY INPUT DO YOU WANT TO INSERT: ");
            scanf("%d", &num);
            
            // consume the newline character left in the input buffer
            getchar();
            
            for (int i = 0; i < num; i++) {
                char s[100], l[100], fa[100], g[100];
                float d;
                
                printf("\n");
                printf("Enter the song name: ");
                scanf(" %[^\n]", s);
                
                printf("Enter the Artist name: ");
                scanf(" %[^\n]", l);
                
                printf("Enter the film/album name: ");
                scanf(" %[^\n]", fa);
                
                printf("Enter the genre name: ");
                scanf(" %[^\n]", g);
                
                printf("Enter the duration of song: ");;
                scanf("%f", &d);
                
                // consume the newline character left in the input buffer
                getchar();
                
                root = insert(root, s, l, fa, g, d);

            }
            
            printf("\n");
            printf("$$$$Insertion Successful$$$$\n");
            printf("\n");
    	}
    	else if(inp==2){
    	    int index = 1;
    	    printf("\n");
    	    inOrder(root, &index);
    	    printf("\n");
    	}
    	else if(inp==3){
    	    int index = 1;
    	    printf("\n");
    	    DeOrder(root, &index);
    	    printf("\n");
    	}
    	else if(inp==4){
    	    char s[100];
    	    printf("\n");
    	    printf("Enter the song name for Deletion: ");
    	    scanf("%s", s);
    	    printf("\n");
    	    deleteNodeWithSongName(&root,s);
    	    printf("\n");
    	}
    	else if(inp==5){
    	    char s[100];
    	    printf("\n");
    	    printf("Enter the Artist name for Deletion: ");
    	    scanf("%s", s);
    	    printf("\n");
    	    deleteNodesWithArtist(&root,s);
    	    printf("\n");
    	}
    	else if(inp==6){
    	    printf("\n");
    	    int i=1;
    	    while(i>0){
    	        int n;
    	        printf("Press 1 to Make a playlist by Singer.\n");;
    	        printf("Press 2 to Make a playlist by Genre.\n");
    	        printf("Press 3 to Make a playlist by Album name.\n");
    	        printf("Press 4 to Make a playlist by Duration.\n");
    	        printf("Press 5 to Make a playlist by Singer name and Album name.\n");
    	        printf("Press 0 to exit.\n");
    	        printf("\n");
    	        printf("Enter your Choice: ");
    	        scanf("%d",&n);
    	        if(n==1){
            	    char nam[15];
            	    printf("\n");
                    printf("Enter the name of the singer whose song's playlist you want to create: ");
                    scanf("%s",nam);
                    printf("\n");
                    newPlayArt(&root,nam);
                    printf("\n");
    	        }
    	        else if(n==2){
    	            char nam[15];
    	            printf("\n");
                    printf("Enter the name of the genre whose song's playlist you want to create: ");
                    scanf("%s",nam);
                    printf("\n");
                    newPlayGen(&root,nam);
                    printf("\n");
    	        }
    	        else if(n==3){
    	            char nam[15];
    	            printf("\n");
                    printf("Enter the name of the album whose song's playlist you want to create: ");
                    scanf("%s",nam);
                    printf("\n");
                    newPlayAlb( &root, nam);
                    printf("\n");
    	        }
    	        else if(n==4){
    	            float nu;
    	            printf("\n");
                    printf("Enter the duration of the song on whose playlist you want to create: ");
                    scanf("%f",&nu);
                    printf("\n");
                    newPlayDu( &root, nu);
                    printf("\n");
    	        }
    	        else if(n==5){
    	            char nam[15],name[20];
    	            printf("\n");
                    printf("Enter the name of the Singer & the Album whose song's playlist you want to create.\n");
                    printf("Name of the Artist: ");
                    scanf("%s", nam);
                    printf("Name of the album: ");
                    scanf("%s", name);
                    printf("\n");
                    newPlayMix( &root, nam, name);
                    printf("\n");
    	        }
    	        else if(n==0){
    	            break;
    	        }
    	    }
            
    	}
    	else if(inp==7){
    	    int index = 1,n,nu;
    	    printf("\n");
    	    inOrder(root, &index);
    	    printf("\n");
    	    printf("Enter the no. of Current song no: ");
    	    scanf("%d",&n);
    	    printf("\n");
    	    printf("Press 0 for previous song.\n");
    	    printf("Press 1 for next song.\n");
    	    printf("\n");
    	    printf("Enter Your Choice: ");
    	    scanf("%d",&nu);
    	    printf("\n");
            display(root,n,nu);
            printf("\n");
    	}
    	else if(inp==8){
    	    int n,k,cho,index = 1;
    	    printf("\n");
    	    inOrder(root, &index);
    	    printf("\n");
    	    printf("Enter the no. of Current song no: ");
    	    scanf("%d",&n);
    	    displayCurrentSong(root,n);
    	    printf("\n");
    	    printf("Enter the value of k: ");
    	    scanf("%d",&k);
    	    printf("Press 0 to Kth Previous song.\n");
    	    printf("Press 1 to Kth Next song.\n");
    	    printf("\n");
    	    printf("Enter Your Choice: ");
    	    scanf("%d",&cho);
    	    if(cho==0){
    	        displayPreviousSong(root,n,k);
    	    }
    	    else if(cho==1){
    	        displayNextSong(root,n,k);
    	    }
    	}
    	else if(inp==9){
    	    int n,index = 1;
    	    printf("\n");
    	    inOrder(root, &index);
    	    printf("\n");
    	    printf("Enter the no. of Current song no: ");
    	    scanf("%d",&n);
    	    struct Node* cur=CurrentSong(root,n);
    	    printf("\n");
    	    while(index>0){
    	        int n;
    	        printf("Press 1 to Make a playlist by Singer.\n");;
    	        printf("Press 2 to Make a playlist by Genre.\n");
    	        printf("Press 3 to Make a playlist by Album name.\n");
    	        printf("Press 4 to Make a playlist by Duration.\n");
    	        printf("Press 0 to exit.\n");
    	        printf("\n");
    	        printf("Enter your Choice: ");
    	        scanf("%d",&n);
    	        if(n==1){
            	    printf("\n");
                    newPlayArt(&root,cur->l);
                    printf("\n");
    	        }
    	        else if(n==2){
    	            printf("\n");
                    newPlayGen(&root,cur->g);
                    printf("\n");
    	        }
    	        else if(n==3){
    	            printf("\n");
                    newPlayAlb( &root, cur->fa);
                    printf("\n");
    	        }
    	        else if(n==4){
                    printf("\n");
                    newPlayDu( &root, cur->d);
                    printf("\n");
    	        }
    	        else if(n==0){
    	            break;
    	        }
    	    }
    	    
    	}
    	else if(inp==10){
    	    printf("\n");
    	    int i=1,n;
    	    while(i>0){
    	        int n;
    	        printf("Press 1 to filter the playlist by Singer & Genre.\n");
    	        printf("Press 2 to filter the playlist by Singer & album.\n");
    	        printf("Press 0 to exit.\n");
    	        printf("\n");
    	        printf("Enter your Choice: ");
    	        scanf("%d",&n);
    	        if(n==1){
    	            char nam[15],name[20];
    	            printf("\n");
                    printf("Enter the name of the Singer & the Genre whose song's playlist you want to create.\n");
                    printf("Name of the Artist: ");
                    scanf("%s", nam);
                    printf("Name of the Genre: ");
                    scanf("%s", name);
                    printf("\n");
                    newPlaySinGen( &root, nam, name);
                    printf("\n");
    	        }
    	        else if(n==2){
    	            char nam[15],name[20];
    	            printf("\n");
                    printf("Enter the name of the Singer & the Album whose song's playlist you want to create.\n");
                    printf("Name of the Artist: ");
                    scanf("%s", nam);
                    printf("Name of the album: ");
                    scanf("%s", name);
                    printf("\n");
                    newPlayMix( &root, nam, name);
                    printf("\n");
    	        }
    	        else if(n==0){
    	            break;
    	        }
    	    }    	       
    	}
    	else if(inp==11){
    	    printf("\n");
            char artist[20];// "John Doe";
            char album[20]; // "Greatest Hits";
            char genre[20]; // "Rock";   
            printf("Enter a set of Attributes. Which will be the basis of Random selected of songs.\n");
            printf("Enter the name of Artist: ");
            scanf("%s",artist);
            printf("Enter the name of the album: ");
            scanf("%s",album);
            printf("Enter the name if the genre: ");
            scanf("%s",genre);
            printf("\n");
            shuffle(root, artist, album, genre);   
    	}
    	else if(inp==12){
    	    printf("\n");
    	    char nam[15],name[20];
            // Perform range-search between ArtistB and ArtistE
            printf("Enter the Initial Artist name: ");
            scanf("%s", nam);
            printf("Enter the Final Artist name: ");
            scanf("%s", name);
            printf("\n");
            Range(root, nam, name);
    	}
    	else if(inp==0){
    	    break;
    	}
    }

	
	return 0;
}








