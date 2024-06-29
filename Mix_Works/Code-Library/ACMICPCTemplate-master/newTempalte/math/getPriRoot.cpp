int getPriRoot(int p) {
  if (p==2) return 1;
  int phi = p - 1;
  getFactor(phi);
  for (int g = 2; g < p; ++g) {
    bool flag=1;
    for (int i = 0; flag && i < N; ++i)
      if (power(g, phi/fac[i], p) == 1)
        flag=0;
    if (flag)
      return g;
  }
}

