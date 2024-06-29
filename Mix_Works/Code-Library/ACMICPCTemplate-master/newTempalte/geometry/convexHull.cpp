bool GScmp(Point a, Point b) {
  if (fabs(a.x - b.x) < eps)
    return a.y < b.y - eps;
  return a.x < b.x - eps;
}
void GS(Point p[],int n,Point res[],int &resn) {
  resn = 0;
  int top = 0;
  sort(p,p+n,GScmp);
  if (conPoint(p,n)) {
    res[resn++] = p[0];
    return;
  }
  if (conLine(p,n)) {
    res[resn++] = p[0];
    res[resn++] = p[n-1];
    return;
  }
  for (int i = 0; i < n;)
    if (resn < 2 ||
        (res[resn-1]-res[resn-2])*(p[i]-res[resn-1]) > 0)
      res[resn++] = p[i++];
    else
      --resn;
  top = resn-1;
  for (int i = n-2; i >= 0;)
    if (resn < top+2 ||
        (res[resn-1]-res[resn-2])*(p[i]-res[resn-1]) > 0)
      res[resn++] = p[i--];
    else
      --resn;
  resn--;
}

