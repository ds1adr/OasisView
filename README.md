# OasisView
OASIS file viewer

1. OASIS: Open Artwork System Interchange Standard. (SEMI P-39, 3626)
2. Qt Project
3. C++ 17 or more.
4. FFTW Libaray
  - Mac OS
    * brew install fftw
  - Linux (Ubuntu)
    * sudo apt update
    * sudo apt install libfftw3-dev
5. Linux system needs Qt6Charts library
    * sudo apt install libqt6charts6-dev
6. GNUPlot
  - 2D result is stored in gnuplot file format, when you enter 'filename' then, it generates 'filename.plt', to see the file:
    * gnuplot -p filename.plt
7. WIP now. Currently parse Rectangle, Trapezoid, CTrapezoid, Text, Placement, Property in the Cell. (6 element types so far) 

Reference:

<img width="958" height="787" alt="Screenshot 2026-04-13 at 6 39 23 PM" src="https://github.com/user-attachments/assets/018fcb73-248a-4ff3-9acb-9a767be9d746" />


WIP:

<img width="889" height="696" alt="Screenshot 2026-05-09 at 10 50 18 PM" src="https://github.com/user-attachments/assets/588dc3d4-70f1-49a5-832b-a68cda14cd8d" />

Zoom In

<img width="889" height="696" alt="Screenshot 2026-05-09 at 10 50 42 PM" src="https://github.com/user-attachments/assets/8fdc84f9-f522-4ad1-a73d-7a51fd69b701" />

Zoom Out

<img width="889" height="696" alt="Screenshot 2026-05-09 at 10 51 21 PM" src="https://github.com/user-attachments/assets/f59386e3-1806-4c61-90a2-fe791221f702" />

Pan

<img width="889" height="696" alt="Screenshot 2026-05-09 at 10 51 47 PM" src="https://github.com/user-attachments/assets/06f85ae0-6aa1-4608-bbec-2a7946ca3bdc" />

Simulation Example (currently hardcoded mask data :))
- Wave Length: 193nm
- NA: 1.35
- Sigma: 0.5 conventional
- 200nm pitch 1:1 Line/Space
  
| Mask | Intensity |
|:-:|:-:|
|<img width="754" height="697" alt="Screenshot 2026-05-26 at 10 59 18 PM" src="https://github.com/user-attachments/assets/55b54b99-76d6-442c-a28a-69d8309eb3a6" />|<img width="754" height="697" alt="Screenshot 2026-05-26 at 10 59 26 PM" src="https://github.com/user-attachments/assets/3e5d6567-935f-4c1b-b04c-b2eb76d8e56d" />|

