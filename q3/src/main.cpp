#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

// 1. Calculate Energy
void compute_energy(int ***rgb, double **energy, int H, int W)
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            // del x
            int rx = rgb[i][(j - 1 + W) % W][0] - rgb[i][(j + 1) % W][0];
            int bx = rgb[i][(j - 1 + W) % W][1] - rgb[i][(j + 1) % W][1];
            int gx = rgb[i][(j - 1 + W) % W][2] - rgb[i][(j + 1) % W][2];

            int delx = rx * rx + bx * bx + gx * gx;

            // del y
            int ry = rgb[(i - 1 + H) % H][j][0] - rgb[(i + 1) % H][j][0];
            int by = rgb[(i - 1 + H) % H][j][1] - rgb[(i + 1) % H][j][1];
            int gy = rgb[(i - 1 + H) % H][j][2] - rgb[(i + 1) % H][j][2];

            int dely = ry * ry + by * by + gy * gy;

            energy[i][j] = sqrt(delx + dely);
        }
    }
}

// 2. Seam Identification
// Vertical Seam

int vertical_seam(double **energy, int H, int W)
{
    double t[H][W];

    for (int i = 0; i < W; i++)
        t[0][i] = energy[0][i];

    for (int i = 1; i < H; i++)
        t[i][0] = energy[i][0] + t[i - 1][0];

    for (int i = 1; i < H; i++)
        for (int j = 1; j < W; j++)
            t[i][j] = energy[i][j] + min(min(t[i - 1][j], t[i - 1][j - 1]), t[i - 1][j + 1]);

    double val = INT64_MAX;
    int idx = INT32_MAX;
    for (int i = 0; i < W; i++)
    {
        if (t[H - 1][i] < val)
        {
            val = t[H - 1][i];
            idx = i;
        }
    }
    cout << val << endl;
    return idx;
}

// 3. Seam Removal

void solve(int ***rgb, int H, int W, int C, int H_, int W_, int C_)
{
    double **energy = new double *[H];

    for (int i = 0; i < H; i++)
        energy[i] = new double[W];

    compute_energy(rgb, energy, H, W);

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            cout << energy[i][j] << "   ";
        cout << endl;
    }

    cout << vertical_seam(energy, H, W) << endl;
}

int main()
{
    string ip_dir = "./data/input/";
    string ip_file = "rgb_in.txt";
    ifstream fin(ip_dir + ip_file);

    int H, W, C;
    fin >> H >> W >> C;

    int ***rgb;
    rgb = new int **[H];
    for (int i = 0; i < H; ++i)
    {
        rgb[i] = new int *[W];
        for (int j = 0; j < W; ++j)
        {
            rgb[i][j] = new int[C];
            for (int k = 0; k < C; ++k)
                fin >> rgb[i][j][k];
        }
    }
    fin.close();

    int H_, W_, C_;
    cout << "Enter new value for H (must be less than " << H << "): ";
    cin >> H_;
    cout << "Enter new value for W (must be less than " << W << "): ";
    cin >> W_;
    cout << '\n';
    C_ = C;

    solve(rgb, H, W, C, H_, W_, C_);

    string op_dir = "./data/output/";
    string op_file = "rgb_out.txt";
    ofstream fout(op_dir + op_file);

    fout << H_ << " " << W_ << " " << C_ << endl;
    for (int i = 0; i < H_; ++i)
    {
        for (int j = 0; j < W_; ++j)
        {
            for (int k = 0; k < C_; ++k)
            {
                fout << rgb[i][j][k] << " ";
            }
        }
        fout << '\n';
    }
    fout.close();

    return 0;
}