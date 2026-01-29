# Use a base image with Node.js and basic build tools
FROM node:20-bookworm

# Install build dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    nasm \
    make \
    binutils \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build C++ Compiler
RUN mkdir -p build && cd build && cmake .. && make

# Build Frontend
WORKDIR /app/playground/client
RUN npm install
RUN npm run build

# Setup Backend
WORKDIR /app/playground/server
RUN npm install

# Expose port (Render sets PORT env var)
EXPOSE 3001

# Start the server
CMD ["node", "index.js"]
