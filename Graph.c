#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define Max 20
typedef char VertexType[Max];
typedef int InfoType;
typedef enum 
{
	DG,DN,UDG,UDN
}GraphKind;

int visited[Max];
void(*VisitFunc)(char* v); 

typedef struct ArcNode
{
	int adjvex;								//vertex position
	struct ArcNode *next;
	InfoType weight;
}ArcNode;

typedef struct VNode
{
	VertexType data;                        //vertex's data
	ArcNode *firstarc;
}VNode,AdjList[Max];

typedef struct 
{
	AdjList vertices;
	int vexnum,arcnum;
	int kind;
}ALGraph;

//Link Queue
typedef struct QNode  
{  
    int data;      
    struct QNode *next; 
}QNode, *QueuePtr;  
  
typedef struct  
{  
    QueuePtr front, rear; 
}LinkQueue;  

//locate vertex
int LocateVex(ALGraph *g,VertexType u)
{
	int i;
	for (i=0;i<g->vexnum;i++)
	{
		if (strcmp(g->vertices[i].data,u)==0)
			return i;
	}
	return -1;
}


//create ALGraph
void CreateGraph(ALGraph *g)
{
	int w;									//weight
	int i,j,k;
	VertexType va,vb;

	printf("Please input the kind of the graph:(0.DG 1.DN 2.UDG 3.UDN)");
	scanf("%d",&g->kind);
	printf("Please input the vexnum and arcnum:");
	scanf("%d %d",&g->vexnum,&g->arcnum);
	printf("Please input the %d vertex's value:",g->vexnum);
	for (i=0;i<g->vexnum;i++)
	{
		scanf("%s",&g->vertices[i].data);
		g->vertices[i].firstarc=NULL;
	}

	if (g->kind==1||g->kind==3)
		printf("Please input the weight,head and tail:");
	else
		printf("Please input the tail and head:");

	for (k=0;k<g->arcnum;k++)
	{
		if (g->kind==1||g->kind==3)
			scanf("%d %s %s",&w,&va,&vb);
		else
			scanf("%s %s",&va,&vb);
		i=LocateVex(g,va);
		j=LocateVex(g,vb);
		ArcNode *p=NULL;
		p = (ArcNode *)malloc(sizeof(ArcNode));
		p->adjvex=j;
		
		if (g->kind==1||g->kind==3)
			p->weight=w;
		else
			p->weight=0;

		p->next=g->vertices[i].firstarc;							//insert to the head of the adjlist
		g->vertices[i].firstarc=p;

		if (g->kind>=2)												//UDG or UDN,create a new node 
		{
			ArcNode *p = NULL;
			p = (ArcNode *)malloc(sizeof(ArcNode));
			p->adjvex=i;

			if (g->kind==1||g->kind==3)
				p->weight=w;
			else
				p->weight=0;

			p->next=g->vertices[j].firstarc;							//insert to the head of the adjlist
			g->vertices[j].firstarc=p;
		}
	}
}

//destroy graph
void Destroy(ALGraph *g)
{
	int i;
	ArcNode *p=NULL,*q=NULL;
	for (i=0;i<g->vexnum;i++)
	{
		p=g->vertices[i].firstarc;
		while (p)
		{
			q=p->next;
			free(p);
			p=q;
		}
	}
}

//get vertex value
VertexType* Getvex(ALGraph g,int v)
{
	if (v>g.vexnum||v<0)
		exit(0);
	return &g.vertices[v].data;
}

//Put a new value for v
void Putvex(ALGraph *g,VertexType v,VertexType value)
{
	int i;
	i=LocateVex(g,v);
	if (i>-1)
		strcpy((*g).vertices[i].data, value);
}

//return first adjvex value
int FirstAdjVex(ALGraph g,VertexType v)
{
	ArcNode *p=NULL;
	int v0;

	v0=LocateVex(&g,v);
	p=g.vertices[v0].firstarc;
	if (p)
		return p->adjvex;
	else
		return -1;
}

//Next adjvex
int NextAdjVex(ALGraph g,VertexType v,VertexType w)
{
	int i;
	ArcNode *p=NULL;
	int v0,w0;

	v0=LocateVex(&g,v);
	w0=LocateVex(&g,w);
	p=g.vertices[v0].firstarc;

	while (p&&p->adjvex!=w0)
		p=p->next;
	if (!p||!p->next)
		return -1;
	return p->next->adjvex;
}

//Insert vertex
void InsertVex(ALGraph *G,VertexType v)
{
	strcpy((*G).vertices[(*G).vexnum].data,v); 
    (*G).vertices[(*G).vexnum].firstarc=NULL;  
    (*G).vexnum++; 
}

//Delete Vertex
void Delete(ALGraph *g,VertexType v)
{
	int i,j=LocateVex(g,v);
	ArcNode *p=NULL,*q=NULL;

	p=g->vertices[j].firstarc;											//delete the arc of this vertex
	while (p)
	{
		q=p;
		p=p->next;
		if (g->kind%2)													//net
			q->weight=0;
		free(q);
		g->arcnum--;
	}

	for (i=j;i<g->vexnum;i++)											//move the vertice
		g->vertices[i]=g->vertices[i+1];
	g->vexnum--;

	for (i=0;i<g->vexnum;i++)
	{
		p=g->vertices[i].firstarc;

		while(p)
		{
			if (p->adjvex==j)
			{
				if (p==g->vertices[i].firstarc)
				{
					g->vertices[i].firstarc=p->next;
					if (g->kind%2)
						p->weight=0;
					free(p);
					if (g->kind<2)										//have direction
						g->arcnum--;
				}
				else
				{
					q->next=p->next;
					if (g->kind%2)
						p->weight=0;
					free(p);
					p=q->next;
					if (g->kind<2)										//have direction
						g->arcnum--;
				}
			}
			else
			{
				if (p->adjvex>j)
					p->adjvex--;
				q=p;
				p=p->next;
			}
		}
	}
}

//Insert Arc
void InsertArc(ALGraph *g,VertexType v,VertexType w)
{
	ArcNode *p=NULL;
	int w1,i,j;
	i=LocateVex(g,v);
	j=LocateVex(g,w);

	p=(ArcNode *)malloc(sizeof(ArcNode));
	p->adjvex=j;
	g->arcnum++;
	//DG
	if (g->kind==0)
	{
		p->next=g->vertices[i].firstarc;
		g->vertices[i].firstarc=p;
	}
	//net
	if (g->kind%2)
	{
		printf("Please input the weight:");
		scanf("%d",&w1);
		p->weight=w1;
		p->next=g->vertices[i].firstarc;
		g->vertices[i].firstarc=p;
	}
	//UD
	if (g->kind>=2)
	{
		p=(ArcNode *)malloc(sizeof(ArcNode));
		p->adjvex=i;
		p->next=g->vertices[j].firstarc;
		g->vertices[j].firstarc=p;
	}
}

//Delete Arc
int DeleteArc(ALGraph *G,VertexType v,VertexType w)  
{  
    ArcNode *p=NULL,*q=NULL;  
    int i,j;  
    i = LocateVex(G,v);  
    j = LocateVex(G,w); 
    if(i < 0 || j < 0 || i == j)  
        return 0;  
    p=G->vertices[i].firstarc;  
    while(p&&p->adjvex!=j)   									//not the arc to be deleted
    {   
        q=p;  
        p=p->next;  
    }  
    if(p&&p->adjvex==j) 										//have found the arc to be deleted 
    {  
        if(p==G->vertices[i].firstarc) 
            G->vertices[i].firstarc=p->next; 
        else  
            q->next=p->next; 
        if(G->kind%2)  
            p->weight=0;  
        free(p);  
        G->arcnum--;    
    }  
    if(G->kind>=2)    											//UDG,UDN
    {  
        p=G->vertices[j].firstarc;    
        while(p&&p->adjvex!=i)  
        {    
            q=p;  
            p=p->next;  
        }  
        if(p&&p->adjvex==i)   
        {  
            if(p==G->vertices[j].firstarc)  
                G->vertices[j].firstarc=p->next;  
            else  
                q->next=p->next;    
            if(G->kind==3) 
                p->weight=0;  
            free(p);   
        }  
    }  
    return 1;  
}  

//Depth First Search from the vth vertex
void DFS(ALGraph g,int v)
{
	int w;
	VertexType v1,w1;  
  
    strcpy(v1,*Getvex(g,v));  
    visited[v] = 1;  
    VisitFunc(g.vertices[v].data); 																//visit the vth vertex
    for(w = FirstAdjVex(g,v1); w >= 0; w = NextAdjVex(g,v1,strcpy(w1,*Getvex(g,w))))  
        if(!visited[w])  
            DFS(g,w);    
}

//Depth First Search
void DFSTraverse(ALGraph G,void(*Visit)(char*))  
{  
    int v;  
    // 使用全局变量VisitFunc,使DFS不必设函数指针参数  
    VisitFunc = Visit;   
      
    for(v = 0; v < G.vexnum; v++)  
        visited[v] = 0;  
    for(v = 0; v < G.vexnum; v++)  
        if(!visited[v])  
            DFS(G,v);      
        printf("\n");  
}  

int InitQueue(LinkQueue *Q)  
{  
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));  
    if(!Q->front)  
        exit(0);  
    Q->front->next = NULL; 
    return 1;  
}  
  
//  insert an element into the rear of the queue  
int EnQueue(LinkQueue *Q, int e)  
{  
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));  
    if( !p )       
        exit(0);  
     
    p->data = e;  
    p->next = NULL;  
   
    Q->rear->next = p;  
    Q->rear = p;  
    return 1;  
}  
  
//  delete the front element of the queue  
int DeQueue(LinkQueue *Q,int *e)  
{  
    QueuePtr p;  
    if(Q->front==Q->rear)  
        return 0;  
    p=Q->front->next;  
    *e=p->data;  
    Q->front->next=p->next;  
    if(Q->rear==p)  
       Q->rear=Q->front;  
    free(p);  
    return 1;  
}  
  

int QueueEmpty(LinkQueue Q)  
{  
    if(Q.front == Q.rear)  
        return 1;  
    else  
        return 0;  
}  
  
//Traverse the graph by Broadth First Search  
void BFSTraverse(ALGraph G,void(*Visit)(char*))  
{  
    int v,u,w;  
    VertexType u1,w1;  
    LinkQueue Q;  
      
    for(v = 0; v < G.vexnum; ++v)  
        visited[v]=0;     
    InitQueue(&Q);            
    for(v = 0; v < G.vexnum; v++)   
        if(!visited[v])              
        {  
            visited[v]=1;  
            Visit(G.vertices[v].data);  
            EnQueue(&Q,v);          
            while(!QueueEmpty(Q))   
            {  
                DeQueue(&Q,&u);       
                strcpy(u1,*Getvex(G,u));  
                for(w = FirstAdjVex(G,u1); w >= 0; w = NextAdjVex(G,u1, strcpy(w1, *Getvex(G,w))))  
                    if(!visited[w]) 
                    {  
                        visited[w] = 1;  
                        Visit(G.vertices[w].data);  
                        EnQueue(&Q,w);     
                    }  
            }  
        }  
        printf("\n");  
}  
 
void print(char *i)
{
	printf("%s ", i);
}

// Display the graph
void Display(ALGraph G)  
{  
    int i;  
    ArcNode *p;  
  
    switch(G.kind)  
    {  
    case DG: printf("DG\n");  
        break;  
    case DN: printf("DN\n");  
        break;  
    case UDG: printf("UDG\n");  
        break;  
    case UDN: printf("UDN\n");  
    }  
    printf("%d vertexs:\n",G.vexnum);  
    for(i = 0; i < G.vexnum; ++i)  
        printf("%s ",G.vertices[i].data);  
    printf("\n%d arcs:\n", G.arcnum);  
    for(i = 0; i < G.vexnum; i++)  
    {  
        p = G.vertices[i].firstarc;  
        while(p)  
        {  
            if(G.kind <= 1)   
            {  
                printf("%s->%s ",G.vertices[i].data,  
                    G.vertices[p->adjvex].data);  
                if(G.kind == DN)    
                    printf(":%d ", p->weight);  
            }  
            else      
            {  
                if(i < p->adjvex)  
                {  
                    printf("%s->%s ",G.vertices[i].data,  
                        G.vertices[p->adjvex].data);  
                    if(G.kind == UDN)      
                        printf(":%d ",p->weight);  
                }  
            }  
            p=p->next;  
        }  
        printf("\n");  
    }  
}  
  
  
int main()  
{  
    int i,j,k,n;  
    ALGraph g;  
    VertexType v1,v2;  
      
    printf("Please selete DG\n");  
    CreateGraph(&g);  
    Display(g);  
      
    printf("Please input the head and tail of the arc you want to delete:\n");  
    scanf("%s%s",v1,v2);  
    DeleteArc(&g,v1,v2);  
    Display(g);  
      
    printf("To change the vertex's value,please input the previous and the latter: ");  
    scanf("%s%s",v1,v2);  
    Putvex(&g,v1,v2);  
    Display(g);  
      
    printf("To insert a new vertex,please input the vertex's value: ");  
    scanf("%s",v1);  
    InsertVex(&g,v1);  
    Display(g);  
      
      
    printf("To insert new arc,please inout the arc numbers:");  
    scanf("%d",&n);  
    for(k=0;k<n;k++)  
    {  
        printf("Please input the other vertex's value: ");  
        scanf("%s",v2);  
        printf("For DG,please input the other vertex's direction:(0.Head 1.Tail)");  
        scanf("%d",&j);  
        if(j)  
            InsertArc(&g,v2,v1);  
        else  
            InsertArc(&g,v1,v2);  
    }  
    Display(g);  
      
	printf("Please input the vertex you want to delete:");
    scanf("%s",v1);  
    Delete(&g,v1);  
    Display(g);  
      
    printf("The result of DFSTraverse:\n");  
    DFSTraverse(g,print);  
      
    printf("The result of BFSTraverse:\n");  
    BFSTraverse(g,print);  
    Destroy(&g);  
  
  
    system("pause");  
    return 0;  
}  
