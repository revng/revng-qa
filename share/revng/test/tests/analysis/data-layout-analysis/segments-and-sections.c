__attribute__((weak)) struct {
  int x;
  char padding[1000];
  int y;
} TheData = {
  .x = 7,
  .padding = { 0 },
  .y = 12,
};

int main() {
  return TheData.x + TheData.y;
}
