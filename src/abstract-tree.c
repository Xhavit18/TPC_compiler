#include "abstract-tree.h"

static const char *StringFromKind[] = {
  "Program",
  "VariablesDecl",
  "VariablesList",
  "FuncsDecl",
  "Function",
  "Prototype",
  "Parametres",
  "Champ",
  "Body",
  "DeclVarFunc",
  "Instructions",
  "Return",
  "FunctionCall",
  "Assignement",
  "Condition",
  "If",
  "Then",
  "Else",
  "While",
  "Loop",
  "Or",
  "And",
  "Equals",
  "Order",
  "AddSub",
  "DivStar",
  "Arguments",
  "Not",
  "Structure",
  "ChampCall",
  "IntLiteral",
  "CharLiteral",
  "Identifier",
  
  /* and all other node labels */
  /* The list must coincide with the enum in abstract-tree.h */
  /* To avoid listing them twice, see https://stackoverflow.com/a/10966395 */
};

Node *makeNode(Kind kind) {
  Node *node = malloc(sizeof(Node));
  if (!node) {
    printf("Run out of memory\n");
    exit(1);
  }
  node->kind = kind;
  node-> firstChild = node->nextSibling = NULL;
  node->lineno=lineno;
  return node;
}

void addSibling(Node *node, Node *sibling) {
  Node *curr = node;
  while (curr->nextSibling != NULL) {
    curr = curr->nextSibling;
  }
  curr->nextSibling = sibling;
}

void addChild(Node *parent, Node *child) {
  if (parent->firstChild == NULL) {
    parent->firstChild = child;
  }
  else {
    addSibling(parent->firstChild, child);
  }
}

void deleteTree(Node *node) {
  if (node->firstChild) {
    deleteTree(node->firstChild);
  }
  if (node->nextSibling) {
    deleteTree(node->nextSibling);
  }
  free(node);
}

void printTree(Node *node) {
  static bool rightmost[128]; // current node is rightmost sibling
  static int depth = 0;       // depth of current node
  for (int i = 1; i < depth; i++) { // 2502 = vertical line
    printf(rightmost[i] ? "    " : "\u2502   ");
  }
  if (depth > 0) { // 2514 = up and right; 2500 = horiz; 251c = vertical and right 
    printf(rightmost[depth] ? "\u2514\u2500\u2500 " : "\u251c\u2500\u2500 ");
  }
  printf("%s", StringFromKind[node->kind]);
  switch (node->kind) {
    case IntLiteral: printf(": %d", node->u.integer); break;
    case CharLiteral: printf(": '%c'", node->u.character); break;
    case Identifier: printf(": %s", node->u.identifier); break;
    case VariablesList: printf(": %s", node->u.identifier); break;
    case Champ: printf(": %s", node->u.identifier); break;
    case Prototype: printf(": %s", node->u.identifier); break;
    case Parametres: printf(": %s", node->u.identifier); break;
    case FunctionCall: printf(": %s", node->u.identifier); break;
    case AddSub: printf(": %c", node->u.character); break;
    case DivStar: printf(": %c", node->u.character); break;
    case Order: printf(": %s", node->u.identifier); break;
    case Equals: printf(": %s", node->u.identifier); break;
    case Structure: printf(": %s", node->u.identifier); break;
    case ChampCall: printf(": %s", node->u.identifier); break;
    default: break;
  }
  printf("\n");
  depth++;
  for (Node *child = node->firstChild; child != NULL; child = child->nextSibling) {
    rightmost[depth] = (child->nextSibling) ? false : true;
    printTree(child);
  }
  depth--;
}
