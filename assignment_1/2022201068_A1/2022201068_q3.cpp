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
int vertical_seam(double **energy, int H, int W, double **cost)
{
    for (int i = 0; i < W; i++)
        cost[0][i] = energy[0][i];

    for (int i = 1; i < H; i++)
        cost[i][0] = energy[i][0] + cost[i - 1][0];

    for (int i = 1; i < H; i++)
        for (int j = 1; j < W; j++)
            cost[i][j] = energy[i][j] + min(min(cost[i - 1][j], cost[i - 1][j - 1]), j + 1 < W ? cost[i - 1][j + 1] : INT64_MAX);

    double val = INT64_MAX;
    int idx = INT32_MAX;
    for (int i = 0; i < W; i++)
    {
        if (cost[H - 1][i] < val)
        {
            val = cost[H - 1][i];
            idx = i;
        }
    }
    return idx;
}

// Horizontal Seam
int horizontal_seam(double **energy, int H, int W, double **cost)
{
    for (int i = 0; i < H; i++)
        cost[i][0] = energy[i][0];

    for (int i = 1; i < W; i++)
        cost[0][i] = energy[0][i] + cost[0][i - 1];

    for (int j = 1; j < W; j++)
        for (int i = 1; i < H; i++)
            cost[i][j] = energy[i][j] + min(min(cost[i][j - 1], cost[i - 1][j - 1]), i + 1 < H ? cost[i + 1][j - 1] : INT64_MAX);

    double val = INT64_MAX;
    int idx = INT32_MAX;
    for (int i = 0; i < H; i++)
    {
        if (cost[i][W - 1] < val)
        {
            val = cost[i][W - 1];
            idx = i;
        }
    }
    return idx;
}

// 3. Seam Removal

// Vertical Seam Removal
void vertical_seam_removal(int ***rgb, int H, int W, double **cost, int idx)
{
    for (int i = H - 1; i >= 0; i--)
    {
        for (int j = idx; j < W - 1; j++)
        {
            rgb[i][j] = rgb[i][j + 1];
        }

        if (i - 1 >= 0)
        {

            double top = cost[i - 1][idx];
            double topleft = idx - 1 > 0 ? cost[i - 1][idx - 1] : INT64_MAX;
            double topright = cost[i - 1][idx + 1];

            double mini = min(min(top, topleft), topright);

            if (mini == topright)
                idx = idx + 1;
            else if (mini == topleft)
            {
                idx = idx - 1;
            }
        }
        else
            break;
    }
}

// Horizonal Seam Removal
void horizontal_seam_removal(int ***rgb, int H, int W, double **cost, int idx)
{
    for (int j = W - 1; j >= 0; j--)
    {
        for (int i = idx; i < H - 1; i++)
        {
            rgb[i][j] = rgb[i + 1][j];
        }

        if (j - 1 >= 0)
        {

            double prev = cost[idx][j - 1];
            double prevup = idx - 1 > 0 ? cost[idx - 1][j - 1] : INT64_MAX;
            double prevdown = idx + 1 < H ? cost[idx + 1][j - 1] : INT64_MAX;

            double mini = min(min(prev, prevup), prevdown);

            if (mini == prevup)
                idx = idx - 1;
            else if (mini == prevdown)
            {
                idx = idx + 1;
            }
        }
        else
            break;
    }
}

void solve(int ***rgb, int H, int W, int C, int H_, int W_, int C_)
{
    int initH = H;
    int initW = W;

    // Loop to remove horizontal seams (To reduce height)
    while (H > H_)
    {
        double **energy = new double *[H];

        for (int i = 0; i < H; i++)
            energy[i] = new double[W];

        compute_energy(rgb, energy, H, W);

        double **cost = new double *[H];
        for (int i = 0; i < H; i++)
            cost[i] = new double[W];

        int idx = horizontal_seam(energy, H, W, cost);
        horizontal_seam_removal(rgb, H, W, cost, idx);

        for (int i = 0; i < H; i++)
        {
            delete[] energy[i];
            delete[] cost[i];
        }

        delete[] energy;
        delete[] cost;
        H--;
    }

    // Loop to remove vertical seams (To reduce width)
    H = initH;
    W = initW;
    while (W > W_)
    {

        double **energy = new double *[H];

        for (int i = 0; i < H; i++)
            energy[i] = new double[W];

        compute_energy(rgb, energy, H, W);

        double **cost = new double *[H];
        for (int i = 0; i < H; i++)
            cost[i] = new double[W];

        int idx = vertical_seam(energy, H, W, cost);
        vertical_seam_removal(rgb, H, W, cost, idx);

        for (int i = 0; i < H; i++)
        {
            delete[] energy[i];
            delete[] cost[i];
        }

        delete[] energy;
        delete[] cost;
        W--;
    }
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