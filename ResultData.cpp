#include "ResultData.h"


ResultData::ResultData()
{
    link_result_state=false;
    link_resultwrite_state=false;
    link_param_state=false;
    link_robotset_state=false;
    link_ftp_state=false;
    b_luzhi=false;
    b_send_group_leaser=false;
    ctx_result_dosomeing=DO_NOTHING;
}

ResultData::~ResultData()
{

}
