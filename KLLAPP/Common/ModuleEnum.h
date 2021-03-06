//---------------------------------------------------------------------------
#ifndef ModuleEnumH
#define ModuleEnumH
//----------------------------------------------------------------
#define FR_SYSTEMREAD  "01"  //系统读权限
//系统权限宏定义
#define FR_SYSTEMWRITE "02"  //系统写权限
//---------------------------------------------------------------------------
//窗体权限宏定义
#define FR_FRMEMPLOYEEDATA_SETUSERLINK "31" //设置用户关联
#define FR_FRMEMPLOYEEDATA_SETPOSITION "32" //职位数据导入权限
#define FR_FRMPROJECT_PLANADJUST "38" //计划调整
#define FR_FRMTASKPROGRESS_PRGEVALUATE "31" //进度评估
#define FR_FRMEARNINGLIST_INCHECK "31" //资金收入审核
#define FR_FRMPAYOUTLIST_OUTCHECK "31" //资金支出审核
#define FR_FRMRETLOANLIST_RETURNCHECK "31" //还贷支出审核
#define FR_FRMPRJCOSTLIST_COSTCHECK "31" //成本审核
#define FR_FRMCONTRACT_CONTRACTDEFER "31" //合同延期
#define FR_FRMCONTRACT_CONTRACTSET "32" //合同状态设置
#define FR_FRMCONTRACT_CONTRACTUNDO "33" //合同状态撤销
#define FR_FRMCONTTASK_PAYCHECK "31" //拨付审核权限
#define FR_FRMCONTTASK_COMPELPAY "32" //强制拨付权限
#define FR_FRMCONTTASKS_PAYCHECK "31" //拨付审核权限
#define FR_FRMCONTTASKS_PAYORDER "32"  //合同付款拨付权限
#define FR_FRMCONTTASKS_COMPELPAY "32" //强制拨付权限
#define FF_FRMCONTTASKEDITPAY_MULTICHECK "71" //付款审批功能
#define FF_FRMCONTTASKS_MULTICHECK "71"  //付款审批功能
#define FR_FRMDOCUSEAPPLY_DOCADMIN "31" //借阅审批权限
#define FR_FRMCLIENT_USERDELETE "31" //客户独立删除权限
#define FR_FRMCLIENT_USERNAMECHANGE "32" //客户名称管理权限
#define FR_FRMCLIENT_VIPCHANGE "33" //会员管理权限
#define FR_FRMHOUSING1_FRMUNITSET "31" //租售状态转换
#define FR_FRMSTUFFPURCHASELIST_STOCKCHECK "31" //采购单审核
#define FR_FRMSTUFFPCSREQUISITION_APPLYCHECK "31" //申请单审核
#define FR_FRMSTUFFPCSPLAN_CHECK "31" //物资采购计划审核
#define FR_FRMCONTTYPE_MENUDATARIGHT "31" //合同分类授权权限
#define FR_FRMOTHERMEAS_CHECKRIGHT "31" //审核反审核权限
#define FR_FRMMIDPAYCER_CHECKRIGHT "31" //审核反审核权限
#define FR_FRMCHGSOLUTION_ATTACH1 "31" //附件1的权限
#define FR_FRMCHGSOLUTION_ATTACH2 "32" //附件2的权限
#define FR_FRMCHGSOLUTION_AUDITING "31" //审核权限
#define FR_FRM041ZD_ATTACH1 "31" //附件1的权限
#define FR_FRM041ZD_ATTACH2 "32" //附件2的权限
#define FR_FRM041ZD_AUDITING "33" //审核权限
#define FR_FRM041RG_ATTACH1 "32" //附件1的权限
#define FR_FRM041RG_ATTACH2 "32" //附件2的权限
#define FR_FRM041RG_AUDITING "33" //审核权限
#define FR_FRM041BG_ATTACH1 "31" //附件1的权限
#define FR_FRM041BG_ATTACH2 "32" //附件2的权限
#define FR_FRM041BG_AUDITING "33" //审核权限
#define FR_FRMQUALITYPROBLEMPUNISHBILL_CHECK "31" //审核权限
#define FR_FRMQUALITYPROBLEMRECTIFYNOTICE_CHECK "31" //审核权限
#define FR_FRMQLYACCIDENTRPT_CHECK "31" //审核权限
#define FR_FRMQLYACDTDEALRPT_CHECK "31" //单据审核
#define FR_FRMQLYPATTERNTABLERPT_MRIGHT "31" //目录授权
#define FR_FRMSAFETYPUNISHNOTICE_CHECK "31" //审核
#define FR_FRMPCWBSMODULE_EDIT "31" //编辑权限
#define FR_FRMPRJESTIMATEACCT_DETAIL_PB029CHECK "31" //项目估算审核权限
#define FR_FRMINVPRJBUDGETCHILD_CHECKRIGHT "31" //项目预算审核权限
#define FR_FRMPRJASSUMABLYACCT_CHILD_CHECKRIGHT "31" //项目概算审核权限
#define FR_FRMCONTCLAIM_CHECK "31" //合同变更审核权限
#define FR_FRMUNCONTRACTEXPENDITURELIST_CHECK "31" //审核权限
#define FR_FRMWORKREPORT_VIEWALLDATARIGHT "31" //查看所有数据权限
/*---------------------------------------------------------
计量支付
----------------------------------------------------------*/
#define FR_FRMCONTRQTY_CHECKRIGHT "31" // 合同清单审核权限
#define FR_FRMDAILYQTYIN_CHECKRIGHT "31" //计日工程量审核权限
#define FR_FRMPARTYABAL_CHECKRIGHT "31" //甲供材结算审核权限
#define FR_FRMPRGPAYASS_CHECK "31" //审核功能
#define FF_FRMPRGPAYASS_MULTICHECK "71" //审批功能
#define FF_FRMPRGPAYASSEDIT_MULTICHECK "71" //审批功能
#define FR_FRMBACKPREPAY_CHECKRIGHT "31" //审核反审核权限
#define FR_FRMMIDBAL_CHECKRIGHT "31" //中间计量支付审核权限
#define FR_FRMMIDVISA_DTL_CHECKRIGHT "31" //中间计量签证审核权限
#define FR_FRMCHGSOLUTIONEDIT_JLPRICERIGHT "32" //监理单价权限
#define FR_FRMCHGSOLUTIONEDIT_ZXPRICERIGHT "33" //咨询单价权限
#define FR_FRMCHGSOLUTIONEDIT_YZPRICERIGHT "34" //业主单价权限
/*---------------------------------------------------------
//物资模块
----------------------------------------------------------*/
#define FR_FRMSTUFFSTOCKOUTLIST_CHECKRIGHT "31" //出库数量审核权限
#define FR_FRMSTUFFSTOCKOUTLIST_CHECKPRICE "32" //单价审核权限
#define FR_FRMSTUFFSTOCKINLIST_CHECKRIGHT "31" //入库数量审核权限
#define FR_FRMSTUFFSTOCKINLIST_CHECKPRICE "32" //单价审核权限
#define FR_FRMSTUFFPCSRETURNLIST_CHECKRIGHT "31" //数量审核权限
#define FR_FRMSTUFFPCSRETURNLIST_CHECKPRICE "32" //单价审核权限
#define FR_FRMSTUFFPCSSETTLEMENTLIST_CHECKRIGHT "31" //采购结算单审核权限
#define FR_FRMSTUFFPCSFUNDREP_CHECKRIGHT "31" //采购计划资金申请审核权限
#define FR_FRMSTUSUPPLYNOTE_CHECK "31" //审核权限
/*---------------------------------------------------------------------------
//投资管理
---------------------------------------------------------------------------*/
#define FR_FRMINVFUNDINCEXPPLAN_DETAIL_CHECKRIGHT "31" //资金支付审核权限
//---------------------------------------------------------------------------
#define ME_FRMPRJWORK 1170112//项目工程管理 (科华)
//---------------------------------------------------------------------------
//窗体功能宏定义
#define FF_FRMPROJECT_WLEDIT "72" //关联工程量模板
#define FF_FRMPROJECT_PROJECTRESOURCE "73" //关联资源模块
#define FF_FRMPROJECT_PROJECTCONTRACT "75" //关联合同模块
#define FF_FRMPROJECT_PROJECTSTUFF "76" //关联物资模块
#define FF_FRMPROJECT_PROJECTQUALITY "71" //关联质量模块
#define FF_FRMPROJECT_INVENTPARAM "74" //财务参数
#define FF_FRMPROJECT_PROJECTDOC "77" //关联文档模块
#define FF_FRMDELIVERRESULT_FRUITQUALITY "71" //交付成果质量标准
#define FF_FRMTASKPROGRESS_RESOURCEPRG "71" //资源领用进度
#define FF_FRMTASKPROGRESS_STUFFPRG "73" //物资领用进度
#define FF_FRMTASKPROGRESS_INVENTBILL "72" //调阅项目成本
#define FF_FRMCONTRACT_INVENTPARAM "71" //财务参数
#define FF_FRMCONTTASK_MULTICHECK "71" //拨付的多重审批
#define FF_FRMCONTTASKEDIT_MULTICHECK "71" //审批功能
#define FF_FRMSOURCEUSE_INVENTBILL "71" //调阅项目成本
#define FF_FRMESTATE_INVENTPARAM "71" //财务参数
#define FF_FRMSTUFFSTOCKOUTLIST_INVENTBILL "71" //调阅项目成本
#define FF_FRMEmployeeData_USERLINK "71" //员工关联角色
#define FF_FRMCONTCLAIM_MULTICHECK "71" //合同变更审批
#define FF_FRMCONTCLAIMEDIT_MULTICHECK "71" //审批功能
#define FF_FRMCLIENT_MODIFYRECORD "71" //变更记录功能
#define FF_FRMSKETCHCONT_MULTICHECK "71" //审批功能
#define FF_FRMSKETCHCONTEDIT_MULTICHECK "71" //审批功能
#define FF_FRMSTUFFPCSREQUISITIONEDIT_APPLYOACHECK "71" //申请单OA审批
#define FF_FRMSTUFFPCSREQUISITION_APPLYOACHECK "71" //申请单OA审批
#define FF_FRMSTUFFPCSPLANEDIT_OACHECK "71" //采购计划OA审批
#define FF_FRMSTUFFPCSPLAN_OACHECK "71" //采购计划OA审批
#define FF_FRMCONTRACTFIN_MULTICHECK "71" //合同结算列表审批
#define FF_FRMCONTRACTFINEDIT_MULTICHECK "71" //合同结算编辑审批
#define FF_FRMMESSAGE_SHORTMESSAGE "71"    //短信功能
#define FF_FRMOTHERMEAS_OACHECK "71" //中间计量OA审核
#define FF_FRM041BG_OACHECK "71" //BG计量OA审批
#define FF_FRM041RG_OACHECK "71" //RG计量OA审批
#define FF_FRM041ZD_OACHECK "71" //ZD计量OA审批
#define FF_FRMPD041_OACHECK "71" //中间计量OA审批
#define FR_FRMDOCCONTENT_CANSETRIGHT "31" //文档目录权限设置按钮权限
//---------------------------------------------------------------------------
//功能菜单宏定义
//Project-模块
#define ME_FRMPROJECT  1010100 //进度计划
#define ME_FRMDELIVERRESULT  1010102 //交付成果
#define ME_FRMPRJCOMPLETE  1010103 //项目竣工
#define ME_FRMADJSTATUSSET  1010104 //计划修复
#define ME_FRMTSKFEE  1010105 //任务费用
#define ME_FRMTSKTEAM  1010106 //团队组织
#define ME_FRMTASKMNT  1010110 //任务维护
#define ME_FRMADJHISTORY  1010111 //项目调整历史记录
#define ME_FRMPRJWORKLOAD  1010112 //项目工程量
#define ME_FRMPRJSTUFF  1010113 //项目物资
#define ME_FramePrjConTRACT  1010114 //项目合同
#define ME_FRMPRJFEE  1010115 //项目费用
#define ME_FRMPRJRESC  1010116 //项目资源
#define ME_FRMPRJDOC  1010117 //项目文档
#define ME_FRMTASKWLIMPORT  1010118 //工程量导入
#define ME_FRMQTYLISTTEMPLATE  1010119 //工程量模板
#define ME_FRMQTYLISTTEMPLATEIMPORT  1010120 //工程量模板导入
#define ME_FRMTASKPROGRESS  1010201 //工程进度
#define ME_FRMRESULTSFIGUREPGS  1010203 //形象进度
#define ME_FRMPRJCHGRECORD  1010204 //项目变更
#define ME_FRMPRJSTPBCKRCD  1010206 //停/复工记录
#define ME_FRMQU_PRJCALCOST  1010301 //工程计价总表
#define ME_FRMQU_WLCALCOST  1010302 //工程量计价总表
#define ME_FRMQU_STUFFCALCOST  1010303 //物资计价总表
#define ME_FRMQU_RESCALCOST  1010304 //资源计价总表
#define ME_FRMQU_FEECALCOST  1010305 //费用计价总表
#define ME_FRMQU_PRJTIMELIMIT  1010311 //项目工期查询
#define ME_FRMPALAVER  1010313 //洽商单
#define ME_FRMPRJTYPE  1010401 //项目类型
#define ME_FRMWORKTYPE  1010402 //工作类型
#define ME_FramePrjConTRACTWAY  1010403 //承包方式
#define ME_FRMFEEDEFINE  1010404 //费用定义
#define ME_FRMPRECMGR  1010408 //参数设置
#define ME_FRMMODIFYTYPE  1010409 //变更类型
#define ME_FRMPCWBSMODULE 1010411 //PCWBS分解
//#define ME_FRMMAINQTY 1010123 //主要工程量
//#define ME_FRMBILLMQTYSPREAD 1010124 //清单与主要工程量分摊
//Resource-模块
#define ME_FRMSOURCETYPE  1020101 //资源类型
#define ME_FRMSOURCEDEFINE  1020102 //资源定义
#define ME_FRMTASKSOURCE  1020103 //任务资源
#define ME_FRMSOURCEUSE  1020104 //资源领用
#define ME_FRMSOURCEDEFINETASK  1020105 //资源任务
#define ME_FRMSOURCEDEFINEIMPORT  1020106 //资源导入
#define ME_FRMQU_RESCUSE  1020202 //资源使用状况
#define ME_FRMQU_SRCLOAD  1020203 //资源负荷分析
#define ME_FRMQU_SRCMONTH  1020204 //资源年度报表
//Finance-模块
#define ME_FRMCALCULATIONSUB  1030101 //核算项目
#define ME_FRMBUILDECONOMY  1030102 //技术经济指标
#define ME_FRMSYSCHARGESUBMODEL  1030103 //收支项目
#define ME_FRMSUBJECT  1030104 //科目表
#define ME_FRMACCOUNTS  1030107 //资金帐号
#define ME_FRMWARRANTDETACHRECORD  1030108 //凭证模板
#define ME_FRMPRJCHARGEDEFINE  1030201 //项目收支估算
#define ME_FRMFINANCEREPORTDETAIL_1  1030202 //项目投资估算表
#define ME_FRMFINANCEREPORTDETAIL_2  1030203 //资金来源与应用
#define ME_FRMFINANCEREPORTDETAIL_3  1030204 //项目现金流量表
#define ME_FRMFINANCEREPORTDETAIL_4  1030205 //资产负债表
#define ME_FRMFINANCEREPORTDETAIL_5  1030206 //损益表
#define ME_FRMFINANCEREPORTDETAIL_6  1030207 //自定义报表
#define ME_FRMEARNINGLIST  1030301 //资金收入单
#define ME_FRMPAYOUTLIST  1030302 //资金支出单
#define ME_FRMRETLOANLIST  1030303 //还贷支出单
#define ME_FRMPB015LIST  1030304 //内部转帐
#define ME_FRMPRJINCOMEXPENSESPLAN  1030305 //项目收支计划
#define ME_FRMPZEXPORT  1030306 //凭证导出
#define ME_FRMMONTHCASHFLOW  1030401 //月收支汇总表
#define ME_FRMYEARCASHFLOW  1030402 //年收支汇总表
#define ME_FRMSZMXQUERY  1030405 //收支明细表
#define ME_FRMKEMUBALANCE  1030407 //科目余额表
#define ME_FRMLAMTRETUPLAN  1030408 //贷款还款计划
#define ME_FRMCASHCONTRACT  1030409 //合同拆分明细表
#define ME_FRMPRJCOSTPLAN  1030501 //费用计划
#define ME_FRMPRJCOSTLIST  1030502 //项目费用
#define ME_FRMCOSTAPPORTIONLIST  1030503 //费用分摊
#define ME_FRMMONTHCOSTQUERY  1030504 //费用月报表
#define ME_FRMYEARCOSTQUERY  1030505 //费用年报表
#define ME_FRMCOSTMXQUERY  1030506 //费用明细表
//Material-模块
#define ME_FRMSTUFFCLASS  1050101 //物资目录
#define ME_FRMSTUFFDEFIN  1050102 //物资维护
#define ME_FRMSTUFFDEPOT  1050103 //仓库维护
#define ME_FRMSTUFFTASK  1050104 //任务物资
#define ME_FRMSTUFFSUPPLIER_STU  1050105 //供应商物资
#define ME_FRMSTUFFUNIT  1050106 //辅助计量单位
#define ME_FRMSTUFFDEFINSUP  1050107 //物资供应商
#define ME_FRMSTUFFDEPOTSTUFF  1050108 //仓库物资
#define ME_FRMSTUFFDEFINIMPORT  1050109 //物资导入
#define ME_FRMSTUFFPURCHASELIST  1050201 //物资采购
#define ME_FRMSTUFFPCSRETURNLIST  1050202 //采购退货
#define ME_FRMSTUFFASKPRICE  1050203 //采购询价
#define ME_FRMSTUEXPENSEALLOCATIONLIST  1050204 //费用分摊
#define ME_FRMSTUASKPRICECLASS  1050205 //询价目录
#define ME_FRMSTUFREIGHTADDERSS  1050206 //货运地址
#define ME_FRMSTUFFPCSREQUISITION  1050207 //采购申请
#define ME_FRMSTUFFSTOCKINLIST  1050208 //采购入库
#define ME_FRMSTUFFPCSREQUISITIONEDIT  1050210 //采购申请编辑
#define ME_FRMSTUFFPCSPLANEDIT 1050217 //物资采购计划编辑
#define ME_FRMSTUFFSTOCKINLIST1  1050301 //其它入库
#define ME_FRMSTUFFSTOCKOUTLIST  1050302 //任务领料
#define ME_FRMSTUSTOCKOUTRETURNLIST  1050303 //任务退料
#define ME_FRMSTUFFSTOCKTRANSFERLIST  1050304 //物资调拨
#define ME_FRMSTUFFSCRAPLIST  1050305 //物资报废
#define ME_FRMSTUFFCHECKLIST  1050306 //仓库盘点
#define ME_FRMSTUFFPCSRETURNLIST1  1050307 //其它出库
#define ME_FRMMATERIALREQPLAN  1050401 //物资需求计划
#define ME_FRMPURREQPLAN  1050402 //物资采购计划
#define ME_FRMQU_ITEMSTUFF  1050403 //项目物资查询
#define ME_FRMQU_STOCKQTY  1050501 //库存总帐
#define ME_FRMQU_STUFFUSE  1050502 //物资使用状况
#define ME_FRMQU_STUFFMONTH  1050504 //物资进出月报
#define ME_FRMMATERIAL  1050505 //指导价物资审定汇签单
#define ME_FRMPRICENOTICE  1050506 //指导价物资价格通知单
#define ME_FRMQU_STUFFUSINGBACK  1050507 //物资领用月报
#define ME_FRMSTUFFPCSFUNDREPEDIT 1050205 //采购资金申请编辑


//Evaluate-模块
#define ME_FRMPRJFINESTIMATE  1100101 //项目完成估算EAC
#define ME_FRMQU_PRJCMPLTPBLITY  1100102 //项目按时完成概率
#define ME_FRMQU_PRJDLYPBLITY  1100103 //项目拖期概率分析
#define ME_FRMTSKFINESTIMATE  1100201 //任务完成估算EAC
#define ME_FRMQU_TSKCMPLTPBLITY  1100202 //任务按时完成概率
#define ME_FRMQU_TSKDLYPBLITY  1100203 //任务拖期概率分析
#define ME_FRMQU_TASKRISK  1120101 //任务进度评估
#define ME_FRMTASKEVMS  1120201 //任务EVMS评估
#define ME_FRMTASKLJINDEX  1120301 //任务临界指数评估
#define ME_FRMPRJSUMMARY  1130101 //项目摘要报告
#define ME_FRMTASKSUMMARY  1130102 //任务摘要报告
#define ME_FRMCRITICALTASK  1130103 //关键任务报告
#define ME_FRMMILESTONETASK  1130104 //里程碑任务报告
#define ME_FRMABOUT2STARTTASK  1130105 //即将开工任务报告
#define ME_FRMDELAYEDTASK  1130106 //拖期任务报告
#define ME_FRMvi_PrjDuty  1130110 //项目责任矩阵
//Contract-模块
#define ME_FRMCONTTYPE  1150103 //合同分类
#define ME_FRMCONTBUSINESS  1150104 //事务类型
#define ME_FRMSYSPARAMETER  1150105 //合同参数设置
#define ME_FRMCONTAMTAPPR_TASK  1150106 //变更类型
#define ME_FRMINSDEDUCT  1150107 //代扣款项定义
#define ME_FRMCONTRACT  1150201 //合同录入
#define ME_FRMCONTCLAIM  1150208 //合同变更
#define ME_FRMCONTTASKSUB  1150211 //子合同款项分摊
#define ME_FRMCONTRACTCON  1150213 //任务合同
#define ME_FRMSKETCHCONT  1150214 //拟定合同
#define ME_FRMCONTBILL  1150215 //合同限价单
#define ME_FRMCONTAPPRPROPLAN  1150216 //合同款项拨付计划
#define ME_FRMCONTCLAIMEDIT  1150217 //合同变更穿透编辑
#define ME_FRMCONTTASKEDITPAY 1150225 //合同付款编辑
#define ME_FRMCONTRACTFINEDIT 1150224 //合同结算编辑
#define ME_FRMCONTTASKEDIT  1150218 //合同拨付穿透编辑
#define ME_FRMCONDATARIGHT  1150219 //合同批量授权
#define ME_FRMSKETCHCONTEDIT  1150220 //拟订合同穿透编辑
#define ME_FRMCONTTASK  1150221 //合同收款
#define ME_FRMCONTTASKS  1150222 //合同付款
#define ME_FRMCONTTEMPLET  1150301 //模板目录
#define ME_FRMOBJPAYMENTRECOD  1150302 //合同模板
#define ME_FRMCONTRACTEDIT  1150303 //合同编辑
#define ME_FRMCONTSHARETEMPLET  1150304 //合同分摊模板
#define ME_FRMCONSUMMARY  1150402 //合同台帐
#define ME_FRMAPPROPRIATE  1150403 //拨款汇签单
#define ME_FRMCONTYPEQRY  1150407 //合同分类汇总
#define ME_FramePrjConQRY  1150408 //项目合同汇总
#define ME_FRMCONPAYQRY  1150409 //甲方分类汇总
#define ME_FRMCONTAKEQRY  1150410 //乙方分类汇总
#define ME_FRMCONPAYMONTH  1150411 //合同资金月报
#define ME_FRMCONTRACTAPP  1150412 //申请汇签单
#define ME_FRMCONTINVESTPARMQRY  1150413 //核算项目合同汇总
#define ME_FRMCONMONQRY  1150501 //合同拨付统计
#define ME_FRMMODIFYQRY  1150502 //合同变更统计
#define ME_FRMCONREPAYQRY  1150503 //合同索赔统计
#define ME_FRMCONPRJQRY  1150504 //合同项目统计
#define ME_FRMSUMPLANFACT  1150507 //合同计划实际综合查询
#define ME_FRMCONSEARCH  1150601 //合同资金汇总
#define ME_FRMCONPAYSTATE  1150602 //项目合同资金状况
#define ME_FRMCONPAYDETAIL  1150603 //项目合同资金明细
#define ME_FRMCAPITALPLANALL  1150606 //资金使用总体计划
#define ME_FRMPRJCAPITALPLAN  1150607 //资金使用总体计划汇总
#define ME_FRMCAPITALPLANYEAR  1150608 //资金使用年度计划
#define ME_FRMCAITALPLANMON  1150609 //资金使用月计划表
#define ME_FRMPRJCAPITALPLANY  1150610 //资金使用年计划汇总
#define ME_FRMPRJCAPITALPLANM  1150611 //资金使用月计划汇总

//Base-模块
#define ME_FRMCALENDAR  1170101 //日历
#define ME_FRMEXCEPTDATE  1170102 //节假日
#define ME_FRMMEASUREUNIT  1170103 //计量单位
#define ME_FRMCURRENCYDATA  1170104 //货币
#define ME_FRMCORPORATIONNATURE  1170105 //企业性质
#define ME_FRMCORPORATIONCLASS  1170106 //单位类型
#define ME_FRMCORPORATION  1170107 //相关单位
#define ME_FRMSYSPARAMETER_C  1170108 //系统参数设置
#define ME_FRMATTENTION  1170110 //今日提示
#define ME_FRMDOCEDIT  1170111 //资料编辑
#define ME_FRMCORPORATIONIMPORT  1170112 //相关单位导入
#define ME_FRMSETTLEMENTMODE  1170113 //结算方式
#define ME_FRMTMPEDITOR  1170114 //自定义报表
#define ME_FRMNEWDUTY  1170201 //职务
#define ME_FRMCORPORATIONORGANIZE  1170202 //部门
#define ME_FRMEMPLOYEEDATA  1170204 //员工
#define ME_FRMROLEDATA  1170301 //角色资料
#define ME_FRMROLEEMP  1170302 //角色人员
#define ME_FRMLOGINUSERRELAX  1170303 //登录用户关联
#define ME_FRMOACHKREPORT  1170304 //OA审批报表
#define ME_FRMDATAAUTHORIZE 1170309 //数据授权
//Quality-模块
#define ME_FRMREGULATIONSFILE  1180101 //法规文件
#define ME_FRMACCEPTANCERPTTABLE  1180102 //验收报告表格
#define ME_FRMASSESSRPTTABLE  1180103 //评审报告表格
#define ME_FRMACCEPTANCESTANDARDTASK  1180201 //项目验收标准
#define ME_FRMACCEPTANCESTANDARD  1180202 //交付成果标准
#define ME_FRMQLYDEFECTDEFINE  1180203 //质量缺陷定义
#define ME_FRMQLYCHECKTOOLBOOK  1180204 //质量检测设备台帐
#define ME_FRMQLYCHECKTOOLBOOKEDIT  1180207 //设置质量检测台帐编辑
#define ME_FRMQTYACCEPTENCETASK  1180301 //项目质量验收
#define ME_FRMQTYACCEPTENCE  1180302 //交付成果验收
#define ME_FRMQLYASSESSRPTTASK  1180303 //项目质量评审
#define ME_FRMQLYASSESSRPT  1180304 //交付成果评审
#define ME_FRMQLYDEFECTFOLLOW  1180305 //质量缺陷跟踪
#define ME_FRMQU_PF021  1180306 //设备质量检测计划
#define ME_FRMQLYREPORT  1180401 //项目质量跟踪
#define ME_FRMQUBUGANALYSE  1180402 //质量缺陷分析
#define ME_FRMQLYPATTERNTABLERPT  1180406 //项目质量报表
//Budget-模块
#define ME_FRMRATIONLIBRARY  1190101 //定额库
#define ME_FRMGLJLIB  1190102 //工料机
#define ME_FRMTOTALRATION  1190103 //综合定额
#define ME_FRMGLJLIBIMPORT  1190110 //工料机导入
#define ME_FRMTOTALRATIONIMPORT  1190111 //定额导入
#define ME_FRMTSKQTYUDGET  1190203 //预算清单
#define ME_FRMQU_PG003  1190301 //预算工料机汇总
#define ME_FRMQU_PG009  1190302 //预算清单汇总
//Sale-模块
#define ME_FRMESTATE  1200101 //楼盘资料
#define ME_FRMBUILDING  1200102 //楼阁资料
#define ME_FRMHSG_FUNCTION  1200103 //楼盘基础资料
#define ME_FRMHOUSING  1200104 //单元资料
#define ME_FRMHOUSETASK  1200108 //楼盘任务
#define ME_FRMPH165LIST  1200111 //附属资产维护
#define ME_FRMSALEAFFAIR_1FREE  1200112 //事务定义
//#define ME_FRMESTATSINFOEDIT  1200115 //楼盘信息编辑
#define ME_FRMMARTFORECAST  1200201 //销售计划
#define ME_FRMPAYMODE  1200202 //付款方式
#define ME_FRMSALEPRICEDPLAN  1200203 //定价试算
#define ME_FRMSALEMIXPRICE  1200204 //单元调价
#define ME_FRMSALEAREARDMMODE_1LIST  1200206 //补差方法
#define ME_FRMHOUSERESERVE  1200302 //单元预订
#define ME_FRMSUBSCRIBEBILL  1200303 //销售认购
#define ME_FRMHOUSECANCEL  1200304 //单元退订
#define ME_FRMESTATEMEDIA  1200305 //多媒体演示
#define ME_FRMSALESIGNCONTLIST  1200306 //签约管理
#define ME_FRMSALEADVT1  1200307 //销控表
#define ME_FRMSALERCOMMISSIONEDIT  1200308 //销售员提成
#define ME_FRMSALEADVT  1200309 //内部销控表
#define ME_FRMPH075  1200310 //销控明细表
#define ME_FRMSALEBREAKCONT  1200401 //违约管理
#define ME_FRMSALETRANSFER  1200403 //过户登记
#define ME_FRMSALEAREAREDEEMCOMP  1200404 //楼房面积补差
#define ME_FRMPH164LIST  1200405 //业主入伙登记
#define ME_FRMSALECANCEL  1200406 //客户退房
#define ME_FRMSALETRANAFFAIR_1LIST  1200407 //事务处理
#define ME_FRMQU_SALEAFFAIR  1200408 //销售事务查询
#define ME_FRMSALERECEIVEMONEY  1200501 //收款单
#define ME_FRMINVOICEREG  1200502 //发票登记
#define ME_FRMPAYMENTNOTICE  1200503 //催款单
#define ME_FRMPH073  1200505 //应收汇总表
#define ME_FRMPH074  1200506 //应收明细表
#define ME_FRMSGATHERINGSUM  1200507 //收款汇总表
#define ME_FRMQU_CREDITRETUAMT  1200508 //贷款到帐统计表
#define ME_FRMPH165  1200603 //楼盘销售报表
#define ME_FRMSALEHSGAFDASSETS  1200604 //销售进度报表
#define ME_FRMSALEAFFAIR_1  1200605 //销售员业绩汇总报表
#define ME_FRMSALETRANAFFAIR_1  1200606 //销售员业绩月报表
#define ME_FRMPH069  1200607 //销售员业绩明细报表
#define ME_FRMPH070  1200608 //销售汇总表
#define ME_FRMPH071  1200609 //销售明细表
#define ME_FRMPH072  1200610 //户型销售分析表
#define ME_FRMQU_SALEMONALLRPT  1200611 //销售综合月报表
#define ME_FRMQU_BANK  1200702 //银行按揭统计
#define ME_FRMQU_PH025  1200703 //客户付款查询
#define ME_FRMQU_PH008  1200704 //单元跟踪查询
#define ME_FRMQU_PH034  1200705 //销售收款查询
#define ME_FRMSALECHANGED  1200706 //销售变更查询
#define ME_FRMQU_PH027  1200707 //客户付款明细
#define ME_FRMQU_PH035  1200708 //销售收款报表
#define ME_FRMPH156  1200801 //客户基础资料
#define ME_FRMCLIENT  1200802 //客户资料
#define ME_FRMSALEFOLLOWUP  1200803 //客户跟踪
#define ME_FRMQU_CLIENT  1200804 //客户资料分析
#define ME_FRMQU_CLIENTBUY  1200805 //客户交易统计
#define ME_FRMQU_SALEFOLLOWUP  1200806 //客户意向分析
#define ME_FRMSALELOGLIST  1200901 //销售日志
#define ME_FRMADVTMGR  1200902 //广告管理
#define ME_FRMESTATSINFO  1200903 //周边楼盘信息
#define ME_FRMESTELECTRONICMAP  1200904 //楼盘电子地图
#define ME_FRMESTATSINFOEDIT  1200905 //周边楼盘信息编辑
#define ME_FRMWEBHOUSE  1200906 //房网备案接口
#define ME_FRMSALECONTTPT_FIELD  1200907 //房网备案接口默认值
#define ME_FRMSALECONTBACKUP  1200908 //网上合同模板明细
//Document-模块
#define ME_FRMDOCFILE  1210101 //文档资料
#define ME_FRMDOCCONTENT  1210102 //文档目录
#define ME_FRMDOCCLASS  1210103 //文档类型
#define ME_FRMTSKDOCTASK  1210105 //任务文档
#define ME_FRMDOCFILEIMPORT  1210107 //文档导入
#define ME_FRMDOCDATARIGHT  1210108 //文档批量授权
#define ME_FRMDOCSEARCH  1210201 //文档检索
#define ME_FRMDOCUSEAPPLY  1210202 //文档借阅申请
#define ME_FRMDOCUSE  1210203 //文档借阅与归还
#define FR_FRMDOCDATARIGHT_GETALLDOC "31" //全文档授权
#define ME_FRMDOCFILEEDIT 1210109 //文档资料编辑
//TechDoc-模块
#define ME_FRMPK202  1340101 //技术文档资料
#define ME_FRMPK201  1340102 //技术文档目录
#define ME_FRMPK209  1340103 //技术文档类型
#define ME_FRMPK204TASK  1340104 //任务技术文档
#define ME_FRMPK202IMPORT  1340105 //文档导入
#define ME_FRMTECHDOCDATARIGHT  1340106 //技术文档批量授权
#define ME_FRMTECHDOCSEARCH  1340201 //技术文档检索
#define ME_FRMPK210  1340202 //技术文档借阅申请
#define ME_FRMPK206  1340203 //技术文档借阅归还
//Safe-模块
#define ME_FRMSAFETYREGULATIONSFILE  1220101 //安全法规制度
#define ME_FRMSAFETYMEETING  1220102 //安全会议
#define ME_FRMPN022  1220105 //安全工器具台帐
#define ME_FRMPN022EDIT  1220106 //设备安全检测台帐编辑
#define ME_FRMSAFETYTOOLSLEADEDBOOK  1220201 //安全设施领用台帐
#define ME_FRMSAFETYWORKNOTICE  1220202 //安全施工问题通知
#define ME_FRMSAFETYRECTIFYRETURN  1220203 //安全隐患整改反馈
#define ME_FRMSAFETYWORKRPT  1220204 //安全工作报表
#define ME_FRMPROBLEMNOTICE  1220205 //安全文明施工问题通知单
#define ME_FRMFEEDBACK  1220206 //隐患整改反馈单
#define ME_FRMSAFETYCHECKTOOLBOOK  1220207 //设备安全检测计划
#define ME_FRMSAFETYPROBLEMDEFINE  1220301 //安全问题定义
#define ME_FRMSAFETYPROBLEMFOLLOW  1220302 //安全问题跟踪
#define ME_FRMQUSAFEANALYSE  1220303 //安全问题分析
#define ME_FRMSAFETYPUNISHNOTICEEDIT  1220305 //安全违章处罚通知单
#define ME_FRMPN022EDIT  1220106 //安全检测台账
#define ME_FRMQUALITYPROBLEMPUNISHBILLEDIT  1180502 //处罚单
#define ME_FRMQUALITYPROBLEMRECTIFYNOTICEEDIT  1180504 //整改督办单
#define ME_FRMQLYACCIDENTRPTEDIT  1180507 //报告单
#define ME_FRMQLYACDTDEALRPTEDIT  1180508 //事故处理单
//Cost-模块
#define ME_FRMPRJESTIMATEACCT  1230201 //估算成本
#define ME_FRMPRJCOSTAIM  1230202 //目标成本
#define ME_FRMCOSTAIMANALYZE  1230203 //目标成本分析
#define ME_FRMPRJSHOPDRAWINGCOST  1230204 //施工图成本
#define ME_FRMCOSTAIMHISTORY  1230205 //目标成本历史查询
#define ME_FRMCOSTBALANCE  1230302 //项目成本对比表
#define ME_FRMCOSTMONREP  1230303 //成本月度报表
#define ME_FRMCOSTYEAREP  1230304 //成本年度报表
#define ME_FRMCOSTDETAIL  1230305 //成本明细表
#define ME_FRMCOSTFORECAST  1230306 //成本预测表
#define ME_FRMCOSTCONTRACT  1230307 //合同拆分明细表
#define ME_FRMUNCONTRACTCOSTSORT  1230401 //非合同成本分类
#define ME_FRMUNCONTRACTEXPENDITURELIST  1230402 //非合同成本分摊
//OA-模块
#define ME_FRMFLOWDEFINLIST  1240101 //流程定义
#define ME_FRMWORKDOCTEMPLET  1240102 //办文模板
#define ME_FRMPRJBLOG  1240201 //项目讨论区
#define ME_FRMPRJNOTICE  1240202 //项目公告栏
#define ME_FRMPRJMEETING  1240203 //项目会议
#define ME_FRMMESSAGE  1240301 //消息管理
#define ME_FRMDOCCHECK  1240302 //我的审批
#define ME_FRMWORKDOCCHECKLIST  1240303 //我的办文
#define ME_FRMPMQQ  1240304 //信息中心
#define ME_FRMCOMPANYAFFICHE  1240307 //公司公告栏
#define ME_FRMDOCSTART  1240305 //我的发起
#define ME_FRMWORKDOCCHECKEDIT  1240306 //办文编辑
#define ME_FRMWORKPLAN  1240401 //工作计划
#define ME_FRMMEMO  1240402 //工作备忘录
#define ME_FRMQU_PRJWORKCENT  1240403 //项目工作中心
#define ME_FRMPHONEBOOK  1240404 //通信录
//CostPrj-工程成本
#define ME_FRMPROJECTBUDGET  1250201 //施工成本预算
#define ME_FRMPRJCOSTSETTINGSET  1250203 //项目成本来源设置
#define ME_FRMPRJCOSTYEAR  1250301 //项目施工成本年报
#define ME_FRMPRJCOSTMONTH  1250302 //项目施工成本月报
#define ME_FRMPRJCOSTDETAIL  1250303 //项目施工成本明细表
#define ME_FRMPRJCOSTTOTAL  1250304 //项目施工成本总表
#define ME_FRMPRJCOSTALLSUM  1250305 //项目成本核算总表
//RentMgr-租赁管理
#define ME_FRMTENEMENTINFO  1270101 //租户维护
#define ME_FRMTENANCYCONTRACT  1270202 //租赁合同
#define ME_FRMGATHERREALLY  1270203 //租赁合同收款实际
#define ME_FRMTENPARAM  1270204 //财务接口参数
#define ME_FRMQU_RENTTOTAL  1270301 //租赁汇总报表
#define ME_FRMTENANCYCONTRPT  1270302 //租赁明细报表
//InviteBid-招投标
#define ME_FRMBIDPRJMAINTAIN  1260101 //招标项目
#define ME_FRMBIDPRJMAINTAIN_ITEM  1260102 //招标项目分项维护
#define ME_FRMBIDCOMPANYSETTING  1260103 //投标单位
#define ME_FRMBIDPRJMARKSTANDARD  1260104 //项目评分标准
#define ME_FRMEVALUATIONOFBID  1260105 //评标
#define ME_FRMASSESS  1260106 //定标
#define ME_FRMBIDPRJMAINTAIN_1  1260201 //招标设备
#define ME_FRMPR002  1260202 //工程特征
#define ME_FRMPR002_1  1260203 //市场参考价
#define ME_FRMPR002_2  1260204 //以往合同价
#define ME_FRMPR002_3  1260205 //参考及经历
#define ME_FRMQU_EQUIPMENT  1260206 //设备成本参考报表
#define ME_FRMEQUIPMENTDATAKIND  1260209 //设备物资文件分类
#define ME_FRMEQUIPMENTDOCMTN  1260210 //设备物资文档维护
//HouseBreaker-房屋拆迁
#define ME_FRMGROUNDINFO  1280101 //地块资料
#define ME_FRMYARDINFO  1280102 //院落资料
#define ME_FRMHOUSEINFO  1280103 //房屋资料
#define ME_FRMHOUSEOWNERINFO  1280104 //产权人资料
#define ME_FRMHOUSEHOLDERINFO  1280105 //住户信息
#define ME_FRMHOUSEOWNERKIND  1280201 //基础资料设置
#define ME_FRMOTHERDEMOLISH  1280202 //其他拆迁物
#define ME_FRMDEMOLISHEXPENSEDEFINE  1280203 //拆迁费用项定义
#define ME_FRMDMLEXPCOUNTMOULD  1280204 //房屋拆迁费用计算模板
#define ME_FRMDMLINDEMNIFYTARGET  1280205 //拆迁补偿指标
#define ME_FRMDEMOLISHRESEARCHLIST  1280301 //拆迁调查表
#define ME_FRMOTHERDEMOLISHOFYARD  1280302 //院落其他拆迁物
#define ME_FRMDMLEXPENSEDETAIL  1280303 //房屋拆迁费用计算
#define ME_FRMHOUSEHOLDERINFOIMPORT  1280106 //住户信息导入
// 计量支付
#define ME_FRMDAILYQTYBASEINFO  1290101 //计日工单价表
#define ME_FRMCHGLEVEL  1290102 //变更级别
#define ME_FRMWORKSTF  1290104 //施工方材料
#define ME_FRMENGDIVIDE  1290105 //工程分解
#define ME_FRMMAINQTY  1290106 //主要工程量
#define ME_FRMBILLMQTYSPREAD  1290107 //清单与主要工程量分摊
#define ME_FRMLISTUNITCSU  1290108 //主要材料单位消耗量
#define ME_FRMCONTRQTY  1290201 //合同工程量清单
#define ME_FRMMEASURECYC  1290202 //计量周期
#define ME_FRMCTRCYCPLAN  1290203 //项目工程量周期计划
#define ME_FRMPPLAN  1290204 //计划进度表
#define ME_FRMCONFACTWORK  1290206 //总体计划进度
#define ME_FRMPRJPLANSUM  1290207 //项目进度计划汇总表
#define ME_FRMPRGPAYAPPROVEATT  1290210 //中间签证周期
#define ME_FRMZEROACCOUNT  1290211 //分部分项工程量
#define ME_FRMZEROACCOUNTIMPORT  1290212 //分部分项工程量导入
#define ME_FRMZERO  1290213 //0#台帐
#define ME_FRMZEROIMPORT  1290214 //0#台帐导入
#define ME_FRMMQTYCMPL  1290215 //主要工程量统计表
#define ME_FRMMIDVISA_DTL  1290300 //中间计量签证
#define ME_FRMMIDBAL  1290301 //中间计量支付
#define ME_FRMDAILYQTYIN  1290303 //计日工计量
#define ME_FRMOTHERMEAS  1290304 //其它计量
#define ME_FRMSTFPRCADJ  1290306 //材料价格调整
#define ME_FRMDAILYQTYINEDIT 1290307 //计日工程量编辑
//#define ME_FRMCHGSOLUTIONEDIT  1290308 //中间计量编辑
#define ME_FRMOTHERMEASEDIT  1290311 //其它计量编辑
#define ME_FRMCHGSOLUTION  1290313 //变更申请
#define ME_FRMCHGSOLUTIONA  1290314 //变更令
#define ME_FRMCHGSOLUTIONEDIT  1290316 //变更申请编辑
#define ME_FRMCHGSOLUTIONEDITA  1290317 //变更令编辑
#define ME_FRMMIDVISA_DTLEDIT  1290318 //中间签证编辑
#define ME_FRMPRGPAYASS  1290319 //进度款支付审批表
#define ME_FRMPRGPAYASSEDIT  1290320 //进度款支付审批表编辑
#define ME_FRMMIDBALEDIT 1290333  //中间计量支付编辑
#define ME_FRMCAPITALPLAN  1290401 //资金支付证书
#define ME_FRMMIDPAYCER  1290402 //中期支付证书
#define ME_FRMBILLPAY  1290403 //清单支付
#define ME_FRMMIDBALQRY  1290404 //中间计量支付汇总表
#define ME_FRMCHGSOLUTION_DTLCHANGE  1290405 //工程变更一览表
#define ME_FRMQU_PRJPRICE  1290408 //单价变更一览表
#define ME_FRMPRICECHANGEQRY  1290409 //价格调整报表
#define ME_FRMPRICECHANGESUMQRY  1290410 //价格调整汇总报表
#define ME_FRMDATELYPAYERQRY  1290411 //计日工支付报表
#define ME_FRMBACKPREPAY  1290412 //扣回开工预付款计算表
#define ME_FRMPRJPAYSUM  1290413 //计量支付报表中心
#define ME_FRMPRJPAYSUM2CON  1290414 //计量支付报表中心（施工）
#define ME_FRMLISTUNITCSUQRY  1290415 //主要材料消耗表
#endif

