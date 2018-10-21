#ifndef __CONFIG_H
#define __CONFIG_H

#include <cassert>
#include <cmath>
#include <string>
#include <utility>
#include <vector>
#include "common.h"

#include "../ext/inih/src/INIReader.h"

namespace dramsim3 {

enum class DRAMProtocol {
    DDR3,
    DDR4,
    GDDR5,
    GDDR5X,
    LPDDR,
    LPDDR3,
    LPDDR4,
    HBM,
    HBM2,
    HMC,
    SIZE
};

enum class RefreshPolicy {
    RANK_LEVEL_SIMULTANEOUS,  // impractical due to high power requirement
    RANK_LEVEL_STAGGERED,
    BANK_LEVEL_STAGGERED,
    SIZE 
};

class Config {
   public:
    Config(std::string config_file, std::string out_dir);

    // DRAM physical structure
    DRAMProtocol protocol;
    int channel_size;
    int channels;
    int ranks;
    int banks;
    int bankgroups;
    int banks_per_group;
    int rows;
    int columns;
    int device_width;
    int bus_width;
    int devices_per_rank;
    int BL;

    // Generic DRAM timing parameters
    double tCK;
    int burst_cycle;  // seperate BL with timing since fot GDDRx it's not BL/2
    int AL;
    int CL;
    int CWL;
    int RL;
    int WL;
    int tCCD_L;
    int tCCD_S;
    int tRTRS;
    int tRTP;
    int tWTR_L;
    int tWTR_S;
    int tWR;
    int tRP;
    int tRRD_L;
    int tRRD_S;
    int tRAS;
    int tRCD;
    int tRFC;
    int tRC;
    // tCKSRE and tCKSRX are only useful for changing clock freq after entering
    // SRE mode we are not doing that, so tCKESR is sufficient
    int tCKE;
    int tCKESR;
    int tXS;
    int tXP;
    int tRFCb;
    int tREFI;
    int tREFIb;
    int tFAW;
    int tRPRE;  // read preamble and write preamble are important
    int tWPRE;
    int read_delay;
    int write_delay;

    // LPDDR4 and GDDR5
    int tPPD;
    // GDDR5
    int t32AW;
    int tRCDRD;
    int tRCDWR;

    // pre calculated power parameters
    double act_energy_inc;
    double pre_energy_inc;
    double read_energy_inc;
    double write_energy_inc;
    double ref_energy_inc;
    double refb_energy_inc;
    double act_stb_energy_inc;
    double pre_stb_energy_inc;
    double pre_pd_energy_inc;
    double sref_energy_inc;

    // HMC
    int num_links;
    int num_dies;
    int link_width;
    int link_speed;
    int num_vaults;
    int block_size;  // block size in bytes
    int xbar_queue_depth;

    // System
    std::string address_mapping;
    std::string queue_structure;
    std::string row_buf_policy;
    RefreshPolicy refresh_policy;
    int cmd_queue_size;
    int trans_queue_size;
    int delay_queue_cycles;
    bool enable_self_refresh;
    int sref_threshold;
    bool aggressive_precharging_enabled;
    bool enable_hbm_dual_cmd;

    std::string output_prefix;

    int epoch_period;
    int output_level;
    std::string output_dir;
    std::string stats_file;
    std::string epoch_stats_file;
    std::string stats_file_csv;
    std::string epoch_stats_file_csv;
    std::string histo_stats_file_csv;

    // Computed parameters
    int request_size_bytes;
    int channel_width, rank_width, bankgroup_width, bank_width, row_width,
        column_width;

    bool IsGDDR() const {
        return (protocol == DRAMProtocol::GDDR5 ||
                protocol == DRAMProtocol::GDDR5X);
    }
    bool IsHBM() const {
        return (protocol == DRAMProtocol::HBM ||
                protocol == DRAMProtocol::HBM2);
    }
    bool IsHMC() const { return (protocol == DRAMProtocol::HMC); }
    // yzy: add another function
    bool IsDDR4() const { return (protocol == DRAMProtocol::DDR4); }

    int ideal_memory_latency;

#ifdef THERMAL
    std::string loc_mapping;
    int power_epoch_period;
    int numRowRefresh;       // number of rows to be refreshed for one time
    double amb_temp;         // the ambient temperature in [C]
    double logic_bg_power;   // background power of logic layer
    double logic_max_power;  // max power of logic layer

    double chip_dim_x;
    double chip_dim_y;
    int numXgrids;
    int numYgrids;
    int mat_dim_x;
    int mat_dim_y;
    // 0: x-direction priority, 1: y-direction priority
    int bank_order;
    // 0; low-layer priority, 1: high-layer priority
    int bank_layer_order;
    int RowTile;
    int TileRowNum;
    double bank_asr;  // the aspect ratio of a bank: #row_bits / #col_bits
    std::string epoch_max_temp_file_csv;
    std::string epoch_temperature_file_csv;
    std::string final_temperature_file_csv;
    std::string bank_position_csv;
#endif  // THERMAL

   private:
    INIReader* reader_;
    void CalculateSize();
    DRAMProtocol GetDRAMProtocol(std::string protocol_str);
    int GetInteger(const std::string& sec, const std::string& opt,
                   int default_val) const;
    void InitDRAMParams();
    void InitOtherParams();
    void InitPowerParams();
    void InitSystemParams();
#ifdef THERMAL
    void InitThermalParams();
#endif  // THERMAL
    void InitTimingParams();
    void SanityCheck();
    void SetAddressMapping();
};

}  // namespace dramsim3
#endif
