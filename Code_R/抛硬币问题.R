flip <- function() sample(c("T", "H"), 1)
set.seed(123)
lheads<-0
nheads<-0
flips <- 0
a<-vector("character",0)
while(lheads<3) 
{
  while (nheads < 3)
  { 
    if (flip() == "H") 
    {
      nheads <- nheads + 1
      a<-c(a,"H")
    } 
    else 
    {
      nheads <- 0
      a<-c(a,"T")
    }
    flips <- flips + 1
  }
  if(flip()=="T")
  {
    lheads<-lheads+1
    a<-c(a,"T")
  }
  else 
  {
    lheads<-0 
    nheads<-0
    a<-vector("character",0)
    flips <- 0
    next
  } 
  flips <- flips + 1
}
flips
print(a)