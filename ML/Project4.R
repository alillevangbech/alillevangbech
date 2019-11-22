path <- paste(getwd(),"/data/",sep = "")
path_projmatrix <- paste(path,"projection_matrix.csv",sep="")
path_MLtrain <- paste(path,"MLWeedCropTrain.csv",sep="")

data_projmatrix <- read.csv(path_projmatrix,header = FALSE)
data_MLtrain <- read.csv(path_MLtrain, header = FALSE)

train_x <- matrix(unlist(data_MLtrain[,1:13]),ncol = 13, nrow = nrow(data_MLtrain))
train_y <- unlist(data_MLtrain[,14])
projmatrix <- matrix(unlist(data_projmatrix),ncol = ncol(data_projmatrix), nrow = nrow(data_projmatrix))

train_trans_x <- t((projmatrix)%*%t(train_x))

dat <- data.frame(train_trans_x,train_y)
rbPal <- colorRampPalette(c("red","green"))
dat$Col <- rbPal(2)[as.numeric(cut(dat$train_y,breaks = 2))]
plot(dat$X1,dat$X2, pch = 20,col = dat$Col, xlab = "X1", ylab = "X2")
legend("right",title="Legend",legend=c("Weed","Crop"),col =rbPal(2),pch=20)
title("Scatterplot of Weeds/Crops distribution")


#Problem 2

Center_data <- train_trans_x[1:2,1:2]
kmeans_2 <- kmeans(train_trans_x, centers = Center_data)

plot(dat$X1,dat$X2, pch = 20,col = dat$Col, xlab = "X1", ylab = "X2")
legend("right",title="Legend",legend=c("Weed","Crop","Clusters"),col =c(rbPal(2),"black"),pch=20)
title("Scatterplot of Weeds/Crops with clutsering")
points(kmeans_2$centers[1], kmeans_2$centers[3],cex = 1.8, pch = 16, col = "black")
points(kmeans_2$centers[2], kmeans_2$centers[4], cex = 1.8, pch = 16, col = "black")

newCluster <- kmeans_2$cluster
for (i in 1:length(kmeans_2$cluster)) {
  if (kmeans_2$cluster[i] == 2) {
  newCluster[i] <- 0    
  }
}
count <- 0
for (i in 1:length(newCluster)) {
  if (newCluster[i] == train_y[i]) {
    count <- count + 1
  }
}
#hit ratio 
count/length(kmeans_2$cluster)

