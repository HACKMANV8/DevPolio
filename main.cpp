#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
#include <map>
#include <array>
#include <unistd.h>

struct Disk {
    std::string name;
    std::string node;
    std::string model;
    std::string serial;
    std::string size;
    std::string rota;
};

std::string run_cmd(const std::string &cmd) {
    std::array<char, 1024> buf{};
    std::string out;
    FILE *fp = popen(cmd.c_str(), "r");
    if (!fp) return out;
    while (fgets(buf.data(), (int)buf.size(), fp)) out += buf.data();
    pclose(fp);
    return out;
}

int run_system(const std::string &cmd) {
    return system(cmd.c_str());
}

std::string run_cmd_capture(const std::string &cmd) {
    return run_cmd(cmd);
}

void split_tokens(const std::string &line, std::map<std::string, std::string> &m) {
    std::istringstream ls(line);
    std::string token;
    while (ls >> token) {
        auto eq = token.find('=');
        if (eq == std::string::npos) continue;
        std::string key = token.substr(0, eq);
        std::string val = token.substr(eq + 1);
        if (val.size() && val.front() == '"' && val.back() == '"')
            val = val.substr(1, val.size() - 2);
        m[key] = val;
    }
}

std::vector<Disk> list_disks() {
    std::vector<Disk> disks;
    FILE* fp = popen("lsblk -P -o NAME,TYPE,SIZE,MODEL,SERIAL,ROTA", "r");
    if (!fp) return disks;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        std::string l(line);
        std::map<std::string,std::string> kv;
        std::stringstream ss(l);
        std::string token;
        while (ss >> token) {
            auto eq = token.find('=');
            if (eq == std::string::npos) continue;
            std::string k = token.substr(0, eq);
            std::string v = token.substr(eq + 1);
            if (v.size() && v.front() == '"' && v.back() == '"') v = v.substr(1, v.size()-2);
            kv[k] = v;
        }
        // Also Detect Loop Disks
        if (kv["TYPE"] != "disk" && kv["TYPE"] != "loop") continue;
        Disk d;
        d.name = kv["NAME"];
        d.node = "/dev/" + d.name;
        d.model = kv["MODEL"];
        d.serial = kv["SERIAL"];
        d.size = kv["SIZE"];
        d.rota = kv["ROTA"];
        disks.push_back(d);
    }
    pclose(fp);
    return disks;
}

std::vector<std::string> list_android_devices() {
    std::string res = run_cmd("adb devices -l");
    std::istringstream iss(res);
    std::string line;
    std::vector<std::string> devs;
    while (std::getline(iss, line)) {
        if (line.find("device") != std::string::npos && line.find("List") == std::string::npos)
            devs.push_back(line);
    }
    return devs;
}

void draw_menu(WINDOW *win, int highlight, const std::vector<std::string> &choices) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Select Mode ");
    for (size_t i = 0; i < choices.size(); i++) {
        if ((int)i == highlight)
            wattron(win, A_REVERSE | COLOR_PAIR(1));
        mvwprintw(win, (int)i + 2, 3, "%s", choices[i].c_str());
        if ((int)i == highlight)
            wattroff(win, A_REVERSE | COLOR_PAIR(1));
    }
    wrefresh(win);
}

void draw_disks(WINDOW *win, int highlight, const std::vector<Disk> &disks) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Local Disks ");
    for (size_t i = 0; i < disks.size(); i++) {
        if ((int)i == highlight)
            wattron(win, A_REVERSE | COLOR_PAIR(1));
        mvwprintw(win, (int)i + 2, 2, "%s (%s)", disks[i].node.c_str(), disks[i].model.c_str());
        if ((int)i == highlight)
            wattroff(win, A_REVERSE | COLOR_PAIR(1));
    }
    if (disks.empty()) mvwprintw(win, 2, 2, "No disks found.");
    wrefresh(win);
}

void draw_android(WINDOW *win, const std::vector<std::string> &devs) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Android / USB Devices ");
    int y = 2;
    if (devs.empty()) mvwprintw(win, y++, 2, "No Android devices found.");
    for (auto &d : devs) mvwprintw(win, y++, 2, "%s", d.c_str());
    wrefresh(win);
}

int main() {
    initscr();
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);    
    init_pair(2, COLOR_GREEN, -1);  
    init_pair(3, COLOR_CYAN, -1);   

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);
    noecho();
    cbreak();
    curs_set(0);

    int h, w;
    getmaxyx(stdscr, h, w);

    WINDOW *mainwin = newwin(h - 2, w - 2, 1, 1);
    keypad(mainwin, TRUE);

    std::vector<std::string> menu = {"Local Disks (NVMe, SSD, HDD)", "Android / USB Devices"};
    int menu_choice = 0;
    int stage = 0;

    while (true) {
        if (stage == 0) {
            draw_menu(mainwin, menu_choice, menu);
            int ch = wgetch(mainwin);
            if (ch == KEY_UP && menu_choice > 0) menu_choice--;
            else if (ch == KEY_DOWN && menu_choice < (int)menu.size() - 1) menu_choice++;
            else if (ch == 10 || ch == KEY_ENTER) {
                stage = (menu_choice == 0) ? 1 : 2;
            } else if (ch == 'q' || ch == 'Q') break;
        }

else if (stage == 1) {  // local disks
    auto disks = list_disks();
    int idx = 0;
    static int loopIndex = 0;  
    keypad(mainwin, TRUE);
    nodelay(mainwin, FALSE);

    while (true) {
        // draw and accept input
        draw_disks(mainwin, idx, disks);
        int ch = wgetch(mainwin);

        if (ch == KEY_UP && idx > 0) idx--;
        else if (ch == KEY_DOWN && idx < (int)disks.size() - 1) idx++;
        else if (ch == 'r' || ch == 'R') {
            disks = list_disks();
            if (idx >= (int)disks.size()) idx = (int)disks.size() - 1;
        }
        else if (ch == 'b' || ch == 'B') { stage = 0; break; }
        else if (ch == 'q' || ch == 'Q') { endwin(); return 0; }

        else if (ch == 10 || ch == KEY_ENTER) {
            if (disks.empty()) continue;
            Disk &d = disks[idx];

            std::string confirm_prompt;
            bool is_loop = (d.node.rfind("/dev/loop", 0) == 0);
            if (!d.serial.empty() && !is_loop) {
                confirm_prompt = "Type device SERIAL to confirm wipe:";
            } else {
                confirm_prompt = std::string("Type device node (") + d.node + ") to confirm wipe:";
            }

            werase(mainwin);
            box(mainwin, 0, 0);
            mvwprintw(mainwin, 2, 2, "Selected disk: %s", d.node.c_str());
            mvwprintw(mainwin, 3, 2, "Model: %s", d.model.c_str());
            mvwprintw(mainwin, 4, 2, "Serial: %s", d.serial.c_str());
            mvwprintw(mainwin, 6, 2, "%s", confirm_prompt.c_str());
            wrefresh(mainwin);

            echo();
            curs_set(1);
            char buf[256];
            mvwgetnstr(mainwin, 7, 4, buf, sizeof(buf) - 1);
            noecho();
            curs_set(0);
            std::string confirm(buf);

            bool confirmed = false;
            if (d.serial.empty()) {
    d.serial = "LOOP-" + std::to_string(loopIndex);
}

            if (!d.serial.empty() && !is_loop) {
                if (confirm == d.serial) confirmed = true;
            } else {
                if (confirm == d.node) confirmed = true;
            }

            if (!confirmed) {
                werase(mainwin);
                box(mainwin, 0, 0);
                wattron(mainwin, COLOR_PAIR(1));
                mvwprintw(mainwin, 3, 2, "Serial/node mismatch. Aborting.");
                wattroff(mainwin, COLOR_PAIR(1));
                mvwprintw(mainwin, 5, 2, "Press any key to return.");
                wrefresh(mainwin);
                wgetch(mainwin);
                continue;
            }
            // Determine wipe method
            std::string method;
            if (d.node.find("nvme") != std::string::npos) {
                method = "nvme-format";
            } else if (d.node.find("loop") != std::string::npos) {
                method = "wipefs-zap";
            } else if (d.node.find("sd") != std::string::npos || d.model.find("ATA") != std::string::npos) {
                method = "ata-secure-erase";
            } else {
                method = "overwrite-zero";
            }

            werase(mainwin);
            box(mainwin, 0, 0);
            mvwprintw(mainwin, 2, 2, "Wiping %s", d.node.c_str());
            mvwprintw(mainwin, 3, 2, "Auto-selected method: %s", method.c_str());
            if (!is_loop) mvwprintw(mainwin, 4, 2, "Note: FORCE_REAL=1 will be set to allow operation on real device.");
            mvwprintw(mainwin, 6, 2, "Press ENTER to proceed, b to cancel.");
            wrefresh(mainwin);

            int okc = wgetch(mainwin);
            if (okc == 'b' || okc == 'B') continue;
            if (!(okc == 10 || okc == KEY_ENTER)) continue;

            // Build command. Use FORCE_REAL=1 for non-loop devices.
            std::string cmd;
            if (is_loop) {
                cmd = "sudo bash ./wipe-device.sh " + d.node + " " + method + " > /tmp/sentinel-wipe.log 2>&1";
            } else {
                cmd = "sudo FORCE_REAL=1 bash ./wipe-device.sh " + d.node + " " + method + " > /tmp/sentinel-wipe.log 2>&1";
            }

            werase(mainwin);
            box(mainwin, 0, 0);
            mvwprintw(mainwin, 2, 2, "Running: %s", cmd.c_str());
            mvwprintw(mainwin, 4, 2, "Logs: /tmp/sentinel-wipe.log");
            mvwprintw(mainwin, 6, 2, "Please wait...");
            wrefresh(mainwin);

            int rc = run_system(cmd);

            // Post-wipe: force re-read, then run a quick verification (wipefs & lsblk)
            run_system(std::string("sudo partprobe ") + d.node + " >/dev/null 2>&1 || true");
            std::string wipefs_out = run_cmd_capture(std::string("sudo wipefs ") + d.node + " 2>/dev/null || true");

            auto refreshed = list_disks();
            bool still_exists = false;
            for (auto &x : refreshed) if (x.node == d.node) { still_exists = true; break; }

            // Present results with colors
            werase(mainwin);
            box(mainwin, 0, 0);
            if (rc == 0 && !still_exists) {
                wattron(mainwin, COLOR_PAIR(2));
                mvwprintw(mainwin, 2, 2, "Wipe succeeded and device seems removed: %s", d.node.c_str());
                wattroff(mainwin, COLOR_PAIR(2));
            } else if (rc == 0 && still_exists) {
                wattron(mainwin, COLOR_PAIR(3));
                mvwprintw(mainwin, 2, 2, "Wipe finished, but device still visible (check below): %s", d.node.c_str());
                wattroff(mainwin, COLOR_PAIR(3));
            } else {
                wattron(mainwin, COLOR_PAIR(1));
                mvwprintw(mainwin, 2, 2, "Wipe failed (rc=%d). See log.", rc);
                wattroff(mainwin, COLOR_PAIR(1));
            }

            mvwprintw(mainwin, 4, 2, "wipefs output (if any):");
            int row = 5;
            std::istringstream iss(wipefs_out);
            std::string l;
            while (std::getline(iss, l) && row < getmaxy(mainwin)-2) {
                mvwprintw(mainwin, row++, 4, "%s", l.c_str());
            }
            mvwprintw(mainwin, getmaxy(mainwin)-3, 2, "Log: /tmp/sentinel-wipe.log");
            mvwprintw(mainwin, getmaxy(mainwin)-2, 2, "Press any key to return.");
            wrefresh(mainwin);
            wgetch(mainwin);

            // refresh local disk list for UI
            disks = list_disks();
            if (idx >= (int)disks.size()) idx = (int)disks.size()-1;
        }
    }
}


        else if (stage == 2) {  // Android device wipe
    std::string mode = "fastboot";
    int idx = 0;

    while (true) {
        werase(mainwin);
        box(mainwin, 0, 0);
        wattron(mainwin, COLOR_PAIR(3));
        mvwprintw(mainwin, 1, 2, "=== Android Wipe Utility ===");
        wattroff(mainwin, COLOR_PAIR(3));
        mvwprintw(mainwin, 3, 4, "[1] Detect via fastboot");
        mvwprintw(mainwin, 4, 4, "[2] Detect via adb");
        mvwprintw(mainwin, 6, 4, "r = rescan, b = back, q = quit");
        mvwprintw(mainwin, 8, 4, "Current mode: %s", mode.c_str());
        wrefresh(mainwin);

        int ch = wgetch(mainwin);
        if (ch == '1') mode = "fastboot";
        else if (ch == '2') mode = "adb";
        else if (ch == 'b' || ch == 'B') { stage = 0; break; }
        else if (ch == 'q' || ch == 'Q') { endwin(); return 0; }

        else if (ch == 'r' || ch == 'R' || ch == 10 || ch == KEY_ENTER) {
            werase(mainwin);
            box(mainwin, 0, 0);
            wattron(mainwin, COLOR_PAIR(3));
            mvwprintw(mainwin, 2, 2, "Detecting Android devices (%s)...", mode.c_str());
            wattroff(mainwin, COLOR_PAIR(3));
            wrefresh(mainwin);

            // detect device using detect-android.sh
            std::string detect_cmd = "bash ./detect-android.sh " + mode + " > /tmp/sentinel-detect.log 2>&1";
            int detect_rc = system(detect_cmd.c_str());
            (void)detect_rc;  // silence warning

            // read first serial from detect output
            std::ifstream log("/tmp/sentinel-detect.log");
            std::string serial, line;
            if (log.is_open()) {
                while (std::getline(log, line)) {
                    if (line.find("Found") != std::string::npos) {
                        auto pos = line.find_last_of(' ');
                        if (pos != std::string::npos) serial = line.substr(pos + 1);
                        break;
                    }
                }
                log.close();
            }

            werase(mainwin);
            box(mainwin, 0, 0);
            if (!serial.empty()) {
                wattron(mainwin, COLOR_PAIR(2));
                mvwprintw(mainwin, 2, 2, "Detected device: %s", serial.c_str());
                wattroff(mainwin, COLOR_PAIR(2));
                mvwprintw(mainwin, 4, 2, "Press ENTER to wipe, or b to cancel.");
            } else {
                wattron(mainwin, COLOR_PAIR(1));
                mvwprintw(mainwin, 2, 2, "No %s device detected.", mode.c_str());
                wattroff(mainwin, COLOR_PAIR(1));
                mvwprintw(mainwin, 4, 2, "Press any key to retry or b to go back.");
                wrefresh(mainwin);
                wgetch(mainwin);
                continue;
            }
            wrefresh(mainwin);

            int k = wgetch(mainwin);
            if (k == 'b' || k == 'B') continue;
            if (k != 10 && k != KEY_ENTER) continue;

            // run android-wipe.sh
            werase(mainwin);
            box(mainwin, 0, 0);
            wattron(mainwin, COLOR_PAIR(3));
            mvwprintw(mainwin, 2, 2, "Running android wipe (%s) on device: %s", mode.c_str(), serial.c_str());
            wattroff(mainwin, COLOR_PAIR(3));
            mvwprintw(mainwin, 4, 2, "Logs are being generated...");
            wrefresh(mainwin);

            std::string wipe_cmd = "bash ./android-wipe.sh " + mode + " " + serial + " > /tmp/sentinel-android.log 2>&1";
            int rc = system(wipe_cmd.c_str());

            // post-wipe message
            werase(mainwin);
            box(mainwin, 0, 0);
            if (rc == 0)
                wattron(mainwin, COLOR_PAIR(2));
            else
                wattron(mainwin, COLOR_PAIR(1));

            mvwprintw(mainwin, 3, 2, "Android wipe finished with status: %d", rc);
            mvwprintw(mainwin, 5, 2, "Attestation generated via attest.sh");
            mvwprintw(mainwin, 7, 2, "Log: /tmp/sentinel-android.log");

            wattroff(mainwin, COLOR_PAIR(1));
            wattroff(mainwin, COLOR_PAIR(2));

            mvwprintw(mainwin, 9, 2, "Press any key to return.");
            wrefresh(mainwin);
            wgetch(mainwin);
        }
    }
}
    }
    endwin();
    return 0;
}
