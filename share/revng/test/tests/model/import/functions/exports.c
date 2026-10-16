//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

// A library whose code refers to no function

// Reachable through the export information
int exported(int X) {
  return X + 1;
}

// Not exported: reachable only through the function starts, if any
__attribute__((used)) static int unexported(int X) {
  return X + 2;
}
