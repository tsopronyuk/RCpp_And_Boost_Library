#####################################################
# This script generates a set of polygons that can
# be used to test the Rcpp code in intersectPolygons.cpp
#####################################################
rm(list = ls(all = TRUE))

# Create the List of polygons. 
## Each polygon is an element of the list.
## Each polygon is stored as a matrix.
#install.packages("ggplot2")
library(ggplot2) 

poly1 <- matrix(c(0, 0, 0, 10, 10, 10, 10, 0, 0, 0), 5, 2, byrow = TRUE)
poly2 <- matrix(c(2, 0, 2, 15, 8, 15, 8, 0, 2, 0), 5, 2, byrow = TRUE)
poly3 <- matrix(c(0, 0, 0, 5, 20, 5, 20, 0, 0, 0), 5, 2, byrow = TRUE)
poly4 <- matrix(c(0, 15, 0, 25, 10, 25, 10, 15, 0, 15), 5, 2, byrow = TRUE)
#poly5 <- matrix(c(5, 0, 5, 5, 0, 5, 0, 10, 5, 10, 5, 15, 10, 15, 10, 10, 15, 10, 15, 5, 10, 5, 10, 0, 5, 0), 13, 2, byrow = TRUE)
poly5 <- matrix(c(0, 0, -7.5, 7.5, 0, 15, 7.5, 7.5, 0, 0), 5, 2, byrow = TRUE)

# Here I create one sample list of polygons to test findIntersections() from intersectPolygons.cpp
listOfPolygons1 <- list(poly1, poly5)

# Here I create another sample list of polygons to test findIntersections() from intersectPolygons.cpp
listOfPolygons2 <- list(poly1, poly2, poly3, poly4, poly5)


##### OPTIONAL PLOTING ######
### These lines will plot the two different listOfPolygons so you can get a sense of what they look like. 
## Un comment the lines and run them to see a plot of the polygons using ggplot2

polyOne <- cbind(c(1),poly1)
polyTwo <- cbind(c(2),poly2)
polyThree <- cbind(c(3),poly3)
polyFour <- cbind(c(4),poly4)
polyFive <- cbind(c(5),poly5)
# 

# plotingDataFrame1 <- data.frame(rbind(polyOne,polyTwo,polyThree))
plotingDataFrame2 <- data.frame(rbind(polyOne,polyTwo,polyThree,polyFour,polyFive))
# 

# 
plotingDataFrame2$X1 <- as.factor(plotingDataFrame2$X1)
# 

# # This plots the first set of polygons in listOfPolygons2
ggplot() + geom_polygon(data = plotingDataFrame2, mapping = aes(group = X1, color = X1, fill = X1, x = X2, y = X3), alpha = 1/2) + coord_fixed()

# the following code will call findIntersections() from intersectPolygons.cpp

library(Rcpp)
#load cpp source from current folder
Rcpp::sourceCpp('findIntersectionspolygons.cpp')

outputIntersections1 <- findIntersections(listOfPolygons1) 
outputIntersections1

#intersection poly1 and poly5
plotingDataFrame_1_5 <- data.frame(outputIntersections1[[1]])
ggplot() + geom_polygon(data = plotingDataFrame_1_5, aes(x = X1, y = X2), alpha = 1/2) + coord_fixed()
 


outputIntersections2 <- findIntersections(listOfPolygons2)
outputIntersections2

#intersection poly3 and poly5
plotingDataFrame_3_5 <- data.frame(outputIntersections2[[9]])
ggplot() + geom_polygon(data = plotingDataFrame_3_5, aes(x = X1, y = X2), alpha = 1/2) + coord_fixed()


#intersection poly2 and poly5
plotingDataFrame_2_5 <- data.frame(outputIntersections2[[7]])
ggplot() + geom_polygon(data = plotingDataFrame_2_5, aes(x = X1, y = X2), alpha = 1/2) + coord_fixed()

