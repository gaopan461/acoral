-- ======================================================================================

-- 创建一个combo表项
function ComboItem(t)
	if type(t.Name) ~= "string" then
		error("ComboItem:Require Name or Name must be string")
	end
	if type(t.Value) ~= "number" then
		error("ComboItem:Require Value or Value must be number")
	end
	if type(t.Checked) ~= "nil" and type(t.Checked) ~= "boolean" then
		error("ComboItem:Checked must be boolean")
	end

	_ComboItem = {
		Name = t.Name,
		Value = t.Value,
		Checked = t.Checked or false,
	}

	return _ComboItem
end

-- ======================================================================================

-- 创建一个编辑框（参数控件）
function ParamEdit(t)
	if type(t.Name) ~= "string" then
		error("ParamEdit:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("ParamEdit:Require CName or CName must be string")
	end
	if type(t.Default) ~= "nil" and type(t.Default) ~= "string" then
		error("ParamEdit:Default must be string")
	end
	if type(t.Note) ~= "nil" and type(t.Note) ~= "string" then
		error("ParamEdit:Note must be string")
	end
	if type(t.Width) ~= "nil" and type(t.Width) ~= "number" then
		error("ParamEdit:Width must be number")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("ParamEdit:Width1 must be number")
	end

	_ParamEdit = {
		CtrlType = "Edit",
		Name = t.Name,
		CName = t.CName,
		Default = t.Default or "",
		Note = t.Note or "",
		Width = t.Width or 100,
		Width1 = t.Width1 or 100,
	}

	return _ParamEdit
end

-- 创建一个组合框（参数控件）
function ParamCombobox(t)
	if type(t.Name) ~= "string" then
		error("ParamCombobox:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("ParamCombobox:Require CName or CName must be string")
	end
	if type(t.Confs) ~= "table" then
		error("ParamCombobox:Require Confs or Confs must be table")
	end
	if type(t.Note) ~= "nil" and type(t.Note) ~= "string" then
		error("ParamCombobox:Note must be string")
	end
	if type(t.Width) ~= "nil" and type(t.Width) ~= "number" then
		error("ParamCombobox:Width must be number")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("ParamCombobox:Width1 must be number")
	end

	_ParamCombobox = {
		CtrlType = "Combobox",
		Name = t.Name,
		CName = t.CName,
		Confs = t.Confs,
		Note = t.Note or "",
		Width = t.Width or 100,
		Width1 = t.Width1 or 100,
	}

	return _ParamCombobox
end

-- 创建一个check组合框（参数控件）
function ParamCheckCombo(t)
	if type(t.Name) ~= "string" then
		error("ParamCheckCombo:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("ParamCheckCombo:Require CName or CName must be string")
	end
	if type(t.Confs) ~= "nil" and type(t.Confs) ~= "table" then
		error("ParamCheckCombo:Confs must be string")
	end
	if type(t.Note) ~= "nil" and type(t.Note) ~= "string" then
		error("ParamCheckCombo:Note must be string")
	end
	if type(t.Width) ~= "nil" and type(t.Width) ~= "number" then
		error("ParamCheckCombo:Width must be number")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("ParamCheckCombo:Width1 must be number")
	end

	_ParamCheckCombo = {
		CtrlType = "CheckCombo",
		Name = t.Name,
		CName = t.CName,
		Confs = t.Confs or {},
		Note = t.Note or "",
		Width = t.Width or 100,
		Width1 = t.Width1 or 100,
	}

	return _ParamCheckCombo
end

-- ======================================================================================

-- 创建一项主配置
function MainItem(t)
	if type(t.Name) ~= "string" then
		error("MainItem:Require Name or Name must be string")
	end
	if type(t.Value) ~= "number" then
		error("MainItem:Require Value or Value must be number")
	end
	if type(t.Checked) ~= "nil" and type(t.Checked) ~= "boolean" then
		error("MainItem:Checked must be boolean")
	end
	if type(t.Width) ~= "nil" and type(t.Width) ~= "number" then
		error("MainItem:Width must be number")
	end
	if type(t.Params) ~= "nil" and type(t.Params) ~= "table" then
		error("MainItem:Params must be table")
	end

	_MainItem = {
		Name = t.Name,
		Value = t.Value,
		Checked = t.Checked or false,
		Width = t.Width or 100,
		Params = t.Params or {},
	}

	return _MainItem
end

-- 创建一项主描述（静态文本）
function MainStatic(t)
	if type(t.Name) ~= "string" then
		error("MainStatic:Require Name or Name must be string")
	end
	if type(t.Width) ~= "nil" and type(t.Width) ~= "number" then
		error("MainStatic:Width must be number")
	end

	_MainStatic = {
		Name = t.Name,
		CtrlType = "Static",
		Width = t.Width or 800,
	}

	return _MainStatic
end

-- ======================================================================================

-- 创建Radio配置
function Radio(t)
	if type(t.Confs) ~= "table" then
		error("Radio:Require Confs or Confs must be table")
	end

	_Radio = {
		ConfType = "Radio",
		Confs = t.Confs,
	}

	return _Radio
end

-- 创建带参数的Radio配置
function RadioWithArg(t)
	if type(t.Confs) ~= "table" then
		error("RadioWithArg:Require Confs or Confs must be table")
	end

	_RadioWithArg = {
		ConfType = "RadioWithArg",
		Confs = t.Confs,
	}

	return _RadioWithArg
end

-- 创建Check配置
function Check(t)
	if type(t.Confs) ~= "nil" and type(t.Confs) ~= "table" then
		error("Check:Confs must be table")
	end

	_Check = {
		ConfType = "Check",
		Confs = t.Confs or {},
	}

	return _Check
end

-- 创建带参数的Check配置
function CheckWithArg(t)
	if type(t.Confs) ~= "nil" and type(t.Confs) ~= "table" then
		error("CheckWithArg:Confs must be table")
	end

	_CheckWithArg = {
		ConfType = "CheckWithArg",
		Confs = t.Confs or {},
	}

	return _CheckWithArg
end
