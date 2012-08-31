require "./BaseCfg"

config = {
	TestRadio1 = Radio{
		Confs = {
			MainItem{Name = "测试1", Value = 0, Checked = true,},
			MainItem{Name = "测试2", Value = 1,},
			MainStatic{Name = "-------------------gaopan--------------------",},
			MainItem{Name = "测试3", Value = 2,},
		},
	},
	TestRadio2 = RadioWithArg{
		Confs = {
			MainItem{Name = "测试1", Value = 0,},
			MainItem{Name = "测试2", Value = 1,},
			MainStatic{Name = "-------------------gaopan--------------------",},
			MainItem{
				Name = "测试3",
				Value = 2,
				Checked = true,
				Params = {
					ParamEdit{
						Name = "参数1",
						CName = "param1",
						Default = "10.5",
						Note = "参数1为浮点数",
						Cast = "Float",
					},
					ParamCombobox{
						Name = "参数2",
						CName = "param2",
						Confs = {
							ComboItem{Name = "高攀1", Value = 0,},
							ComboItem{Name = "高攀2", Value = 1,},
							ComboItem{Name = "高攀3", Value = 2, Checked = true,},
							ComboItem{Name = "高攀4", Value = 3,},
							ComboItem{Name = "高攀5", Value = 4,},
							ComboItem{Name = "高攀6", Value = 5,},
						},
						Note = "组合框测试，默认值为高攀3",
					},
					ParamCheckCombo{
						Name = "参数3",
						CName = "param3",
						Confs = {
							ComboItem{Name = "美艳1", Value = 0,},
							ComboItem{Name = "美艳2", Value = 1, Checked = true,},
							ComboItem{Name = "美艳3", Value = 2,},
							ComboItem{Name = "美艳4", Value = 3, Checked = true,},
						},
						Note = "组合复选框，默认为美艳2和美艳4",
					},
				},
			},
		},
	},
	TestCheck1 = Check{
		Confs = {
			MainItem{Name = "测试1", Value = 0, Checked = true,},
			MainItem{Name = "测试2", Value = 1,},
			MainStatic{Name = "-------------------gaopan--------------------",},
			MainItem{Name = "测试3", Value = 2,Checked = true,},
		},
	},
	TestCheck2 = CheckWithArg{
		Confs = {
			MainItem{Name = "测试1", Value = 0, Checked = true,},
			MainItem{Name = "测试2", Value = 1,},
			MainStatic{Name = "-------------------gaopan--------------------",},
			MainItem{
				Name = "测试3",
				Value = 2,
				Checked = true,
				Params = {
					ParamEdit{
						Name = "参数1",
						CName = "param1",
						Default = "10.5",
						Note = "参数1为浮点数",
						Cast = "Float",
					},
					ParamCombobox{
						Name = "参数2",
						CName = "param2",
						Confs = {
							ComboItem{Name = "高攀1", Value = 0,},
							ComboItem{Name = "高攀2", Value = 1,},
							ComboItem{Name = "高攀3", Value = 2, Checked = true,},
							ComboItem{Name = "高攀4", Value = 3,},
							ComboItem{Name = "高攀5", Value = 4,},
							ComboItem{Name = "高攀6", Value = 5,},
						},
						Note = "组合框测试，默认值为高攀3",
					},
					ParamCheckCombo{
						Name = "参数3",
						CName = "param3",
						Confs = {
							ComboItem{Name = "美艳1", Value = 0,},
							ComboItem{Name = "美艳2", Value = 1, Checked = true,},
							ComboItem{Name = "美艳3", Value = 2,},
							ComboItem{Name = "美艳4", Value = 3, Checked = true,},
						},
						Note = "组合复选框，默认为美艳2和美艳4",
					},
				},
			},
		},
	},
}

-- print(config.TestRadio1.Confs[1].Name)
