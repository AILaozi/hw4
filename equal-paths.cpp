#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int getHeight(Node* root) {
  if (root == nullptr) return 0;
  int height_left = getHeight(root->left);
  int height_right = getHeight(root->right);

  if (height_left > height_right) {
    return 1 + height_left;
  }
  else {
    return 1 + height_right;
  }
}

bool equalPaths(Node * root)
{
  // Add your code below

  if (root == nullptr) return true;
  if (root->left == nullptr && root->right == nullptr) return true;

  if (root->left == nullptr) return equalPaths(root->right);
  if (root->right == nullptr) return equalPaths(root->left);
  
  int left = getHeight(root->left);
  int right = getHeight(root->right);

  if (left != right) return false;

  return equalPaths(root->left) && equalPaths(root->right);
}

