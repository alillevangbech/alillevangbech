path <- paste(getwd(),"data","DanWoodOriginal.dt.txt",sep="/")
data <- read.table(path)

### Linear regression algorithm
LinearRegressionAlgo <- function(x_train,y_train){
  if (is.vector(x_train)) {
    x_tilde <- matrix(1,nrow = length(x_train),ncol = 2)
    x_tilde[,1] = x_train
  } else if (is.matrix(x_train)) {
    x_tilde <- cbind(x_train,rep(1,nrow(x_train)))
  }
  w <- qr.solve(t(x_tilde)%*%x_tilde)%*%t(x_tilde)%*%y_train
  return (w)
}

### Affine linaer model
AffineLinearModel <- function(x,x_train,y_train){
  w <- LinearRegressionAlgo(x_train,y_train)
  x_return <- matrix(1, nrow = length(x), ncol = 2)
  x_return[,1] = x 
  return (x_return%*%w)
}

xData <- data[,1]
yData <- data[,2]

## Specific Affine linear model for my data - returns prediction based on x
M1 <- function(x){
    return (AffineLinearModel(x,xData,yData))
}
MSE_M1 <- mean((yData-M1(xData))^2)
## Model parameters:
Parameters_M1 <- rbind(LinearRegressionAlgo(xData,yData),MSE_M1)
rownames(Parameters_M1) <- c('a','b','MSE_M1')
colnames(Parameters_M1) <- "Parameters"
Parameters_M1
yDataVar <- var(yData)
yDataVar


plot(xData,yData, xlab = "Absolute temperature", ylab = "Energy Radiated", sub = "Figure 8")
curve(M1,add = TRUE)
text(1.36,5.5,paste("MSE =",toString(format(MSE_M1, digits = 2)), sep = " "))
title("Linear model: Plot of regression line")

quotient <- MSE_M1/yDataVar
quotient

xTransformData <- xData^3
M2 <- function(x){
  return (AffineLinearModel(x,xTransformData,yData))
}
MSE_M2 <- mean((yData-M2(xTransformData))^2)
MSE_M2
plot(xTransformData,yData,xlab = "Transformed Absolute temperature", ylab = "Energy Radiated", sub = "Figure 9")
curve(M2,add = TRUE)
text(2.4,5.5,paste("MSE =",toString(format(MSE_M2, digits = 4)), sep = " "))
title("Non-linear model: Plot of regression line")





