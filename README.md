# 🎵 MP3 Tag Reader in C

🔍 *“Read and edit MP3 metadata with ease.”*

---

## 📖 About the Project
This project implements an **MP3 Tag Reader** in **C programming**.  
It allows users to read and edit metadata (ID3 tags) from MP3 files, such as:

- Title  
- Artist  
- Album  
- Year  

The project focuses on **file handling**, **binary data reading**, and **low-level programming** concepts, making it ideal for students and engineers interested in **systems programming**.

---

## ✨ Key Highlights
- 🗂️ Reads MP3 metadata (ID3v1/v2)  
- ✏️ Allows editing and saving changes  
- 💻 Implemented entirely in **C**  
- ⚡ Efficient file handling and parsing  
- 🖥️ Linux / GCC compatible  

---

## 🧠 Technical Concept
MP3 files store metadata in **ID3 tags**.  

- **ID3v1:** Fixed-size tag at the end of the file (128 bytes)  
- **ID3v2:** Variable-size tag at the start of the file  

This project reads these tags, interprets the data, and allows modification without affecting the audio content.

---

## ⚙️ How It Works
1. Load the MP3 file  
2. Check for ID3v1 or ID3v2 tags  
3. Parse the tag fields (Title, Artist, Album, Year)  
4. Display metadata to the user  
5. Optionally edit fields and save changes  

---

## 🚀 How to Compile and Run

### 🔹 Compile
```bash
gcc *.c -o mp3reader

