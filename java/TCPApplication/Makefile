JAVAC=javac
JAR=jar
BUILD_DIR=build
DIST_DIR=dist
CLASSES=TCPServer TCPClient

all: dirs compile jar

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DIST_DIR)

compile:
	@for class in $(CLASSES); do \
		$(JAVAC) -d $(BUILD_DIR) src/$$class.java; \
	done

jar:
	@for class in $(CLASSES); do \
		$(JAR) cfe $(DIST_DIR)/$$class.jar $$class -C $(BUILD_DIR) $$class.class; \
	done

clean:
	@rm -rf $(BUILD_DIR) $(DIST_DIR)

